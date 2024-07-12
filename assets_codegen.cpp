#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

using substitutions = std::map<std::string, std::string>;

std::string do_substitutions(const std::string &in,
                             const substitutions &subst) {
  std::ostringstream out;
  size_t pos = 0;
  for (;;) {
    size_t subst_pos = in.find("{{", pos);
    size_t end_pos = in.find("}}", subst_pos);
    if (end_pos == std::string::npos)
      break;

    out.write(&*in.begin() + pos, subst_pos - pos);

    subst_pos += strlen("{{");
    substitutions::const_iterator subst_it =
        subst.find(in.substr(subst_pos, end_pos - subst_pos));
    if (subst_it == subst.end())
      throw std::runtime_error("undefined substitution");

    out << subst_it->second;
    pos = end_pos + strlen("}}");
  }
  out << in.substr(pos, std::string::npos);
  return out.str();
}

substitutions get_substitutions(int argc, char **argv) {
  substitutions subs;

  for (int i = 1; i < argc; i++) {
    std::string argument{argv[i]};
    size_t split_pos = argument.find("=");
    size_t value_length = argument.size() - split_pos - 1;
    if (split_pos == std::string::npos)
      continue;
    subs.emplace(std::string{&argv[i][0], split_pos},
                 std::string{&argv[i][split_pos + 1], value_length});
  }

  return subs;
}

std::string get_in(int argc, char **argv) {
  std::ifstream input(argv[1]);
  std::ostringstream in;
  in << input.rdbuf();
  return in.str();
}

int main(int argc, char **argv) try {
  if (argc < 3)
    throw std::runtime_error("invalid number of arguments");

  std::ofstream output(argv[2]);
  output << do_substitutions(get_in(argc, argv),
                                get_substitutions(argc, argv));
  return 0;
} catch (const std::exception& error) {
  std::cout << "Usage: " << argv[0] << " input output [key=value...]" << std::endl;
  std::cerr << "Error: " << error.what() << std::endl;
  return 1;
}
