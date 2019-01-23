function out = jsondecode_test
  str = '[[1, 2.3456, null], {"foo": 42, "bar": "a string", "baz": [1 2 3], "qux": 1.23]';
  str = '[[1, 2.3456, null], {"foo": 42, "bar": "a string", "baz": [1 2 3], "qux": 1.23}]';
  str = '[[1, 2.3456, null], {"foo": 42, "bar": "a string", "baz": [1, 2, 3], "qux": {"whatever": [1, 42, null]}}]';
  out = jsondecode (str);
endfunction
