    operator tinyobj::ObjReader() const {
      char source[size + 1];
      strncpy(source, data, size);
      source[size] = '\0';
      tinyobj::ObjReader reader;
      reader.ParseFromString(source, "");
      return reader;
    }
