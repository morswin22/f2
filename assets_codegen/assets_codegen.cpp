#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_set>

using substitutions_t = std::map<std::string, std::string>;

std::string do_substitutions(const std::string &in,
                             const substitutions_t &subst) {
  std::ostringstream out;
  size_t pos = 0;
  for (;;) {
    size_t subst_pos = in.find("{{", pos);
    size_t end_pos = in.find("}}", subst_pos);
    if (end_pos == std::string::npos)
      break;

    out.write(&*in.begin() + pos, subst_pos - pos);

    subst_pos += strlen("{{");
    substitutions_t::const_iterator subst_it =
        subst.find(in.substr(subst_pos, end_pos - subst_pos));
    if (subst_it == subst.end())
      throw std::runtime_error("undefined substitution");

    out << subst_it->second;
    pos = end_pos + strlen("}}");
  }
  out << in.substr(pos, std::string::npos);
  return out.str();
}

std::string get_file(const std::filesystem::path &path) {
  std::ifstream input(path);
  std::ostringstream in;
  in << input.rdbuf();
  return in.str();
}

static std::filesystem::path template_dir;
static std::string target;

struct mime_handler {
  std::unordered_set<std::string> extensions;
  std::unordered_set<std::string> headers;
  mime_handler(const std::unordered_set<std::string> &extensions,
               const std::unordered_set<std::string> &headers)
      : extensions(extensions), headers(headers) {}
  virtual ~mime_handler() {}
  virtual std::string
  get_header_asset(const std::filesystem::path &path) const = 0;
  virtual std::string
  get_source_asset(const std::filesystem::path &path) const {
    return do_substitutions(get_file(template_dir / "source_template.cpp"),
                            substitutions_t{{"target", target},
                                            {"path", path},
                                            {"name", path.filename()}});
  }
};

struct image_handler : mime_handler {
  image_handler()
      : mime_handler({".jpg", ".jpeg", ".png", ".bmp"},
                     {"f2/asset/image.hpp"}) {}
  std::string
  get_header_asset(const std::filesystem::path &path) const override {
    return get_file(template_dir / "image_template.hpp");
  };
};

struct shader_source_handler : mime_handler {
  shader_source_handler()
      : mime_handler({".vert", ".frag", ".geom"},
                     {"f2/render/shader.hpp", "cstring"}) {}
  std::string get_shader_type(const std::string &extension) const {
    if (extension == ".vert")
        return "VERTEX";
    if (extension == ".frag")
        return "FRAGMENT";
    if (extension == ".geom")
        return "GEOMETRY";
    throw std::runtime_error("unknown shader source type");
  }
  std::string
  get_header_asset(const std::filesystem::path &path) const override {
    return do_substitutions(
        get_file(template_dir / "shader_source_template.hpp"),
        substitutions_t{{"shader_type", get_shader_type(path.extension())}});
  };
};

struct obj_handler : mime_handler {
  obj_handler() : mime_handler({".obj"}, {"tiny_obj_loader.h", "cstring"}) {}
  std::string
  get_header_asset(const std::filesystem::path &path) const override {
    return get_file(template_dir / "obj_template.hpp"); // TODO: mtl support
  };
};

int main(int argc, char **argv) try {
  if (argc != 5)
    throw std::runtime_error("invalid number of arguments");

  target = argv[4];

  template_dir = argv[3];
  if (!std::filesystem::exists(template_dir) ||
      !std::filesystem::is_directory(template_dir))
    throw std::runtime_error(
        "argument 'template' either doesn't exist or is not a directory");

  std::filesystem::path output_dir(argv[2]);
  if (!std::filesystem::exists(output_dir))
    std::filesystem::create_directories(output_dir);

  std::unordered_set<std::string> headers, assets;

  std::array<std::unique_ptr<mime_handler>, 3> handlers{
      std::make_unique<image_handler>(),
      std::make_unique<shader_source_handler>(),
      std::make_unique<obj_handler>()};

  const std::string asset_header_template =
      get_file(template_dir / "asset_template.hpp");

  for (const auto &entry : std::filesystem::directory_iterator(argv[1])) {
    auto input_path = entry.path();

    for (const auto &handler : handlers)
      if (handler->extensions.contains(input_path.extension())) {
        headers.merge(handler->headers);

        assets.insert(do_substitutions(
            asset_header_template,
            substitutions_t{{"name", input_path.filename()},
                            {"casts", handler->get_header_asset(input_path)}}));

        std::ofstream output(
            output_dir /
            (static_cast<std::string>(input_path.filename()) + ".cpp"));
        output << handler->get_source_asset(input_path);

        break;
      }
  }

  std::ostringstream headers_stream;
  for (const auto &header : headers)
    headers_stream << "#include <" << header << ">\n";

  std::ostringstream assets_stream;
  std::copy(assets.begin(), assets.end(),
            std::ostream_iterator<std::string>(assets_stream, "\n"));

  std::ofstream header(output_dir / "f2_assets.hpp");
  header << do_substitutions(get_file(template_dir / "header_template.hpp"),
                             substitutions_t{{"headers", headers_stream.str()},
                                             {"assets", assets_stream.str()}});

  return 0;
} catch (const std::exception &error) {
  std::cout << "Usage: " << argv[0] << " input output template target" << std::endl;
  std::cerr << "Error: " << error.what() << std::endl;
  return 1;
}
