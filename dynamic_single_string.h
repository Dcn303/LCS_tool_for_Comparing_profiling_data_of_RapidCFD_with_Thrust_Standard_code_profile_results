 //*************************dynamic_single_string.h*************************//
  #ifndef DYNAMIC_SINGLE_STRING_H
  #define DYNAMIC_SINGLE_STRING_H
    struct dynamic_single_string
    {
            char *string;
            int total_string_char;
    };
  int add_char(struct dynamic_single_string *,char );
  void initialization_dynamic_string(struct dynamic_single_string *);
  #endif
