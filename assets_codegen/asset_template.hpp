  template <>
  struct asset<"{{name}}"> {
    static const char* data;
    static const size_t size;
    
  {{casts}}
  };
