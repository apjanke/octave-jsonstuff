/*
Copyright (C) 2019 Andrew Janke <floss@apjanke.net>

This file is part of Octave.

Octave is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Octave is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<https://www.gnu.org/licenses/>.
*/

#include <cmath>
#include <iostream>
#include <sstream>

#include <json/json.h>
#include <octave/oct.h>

// Inputs:
//   1: Encoded JSON data as char vector
// Outputs:
//   1: Decoded value as Octave value

// Prototypes

octave_value
decode_recursive (Json::Value jval);

// Definitions

octave_value
decode_double (Json::Value jval) {
  NDArray out (dim_vector (1, 1));
  out(0) = jval.asDouble ();
  return out;
}

octave_value
decode_array (Json::Value jval) {
  // Decode all the elements first, and then decide how to combine them,
  // based on the set of element types.
  // All JSON values are effectively doubles, so we can just ignore the Int/UInt stuff.
  bool is_all_numeric = true;
  for (int i = 0; i < jval.size(); i++) {
  	Json::Value v = jval[i];
  	if (! (v.isNumeric () || v.isNull ())) {
      is_all_numeric = false;
      break;
  	}
  }
  if (is_all_numeric) {
  	NDArray out (dim_vector (1, jval.size()));
  	for (int i = 0; i < jval.size(); i++) {
  	  if (jval[i].isNull ()) {
  	  	out(i) = NAN;
  	  } else {
  	    out(i) = jval[i].asDouble();
  	  }
  	}
  	return out;
  } else {
  	Cell out (dim_vector (1, jval.size()));
  	for (int i = 0; i < jval.size(); i++) {
  	  out(i) = decode_recursive (jval[i]);
  	}
  	// TODO: condense same-fielded object/struct array
  	return out;
  }
}

octave_value
decode_object (Json::Value jval) {
  octave_map s;
  Json::Value::Members members = jval.getMemberNames ();
  for (size_t i = 0; i < members.size (); i++) {
  	s.assign (members[i], decode_recursive (jval[members[i]]));
  }
  return s;
  // return octave_value ("<unimplemented JSON type: object>");
}

octave_value
decode_string (Json::Value jval) {
  return octave_value (jval.asString ());
}

octave_value
decode_boolean (Json::Value jval) {
  boolNDArray out = boolNDArray (dim_vector (1, 1));
  out(0) = jval.asBool ();
  return out;
}

octave_value
decode_null (Json::Value jval) {
  NDArray out (dim_vector (0, 0));
  return out;
}

octave_value
decode_recursive (Json::Value jval) {
  switch (jval.type ()) {
  	case Json::nullValue:
  	  return decode_null (jval);
  	  break;
  	case Json::intValue:
  	  return decode_double (jval);
  	  break;
  	case Json::uintValue:
  	  return decode_double (jval);
  	  break;
  	case Json::realValue:
  	  return decode_double (jval);
  	  break;
  	case Json::stringValue:
  	  return decode_string (jval);
  	  break;
  	case Json::booleanValue:
  	  return decode_boolean (jval);
  	  break;
  	case Json::arrayValue:
  	  return decode_array (jval);
  	  break;
  	case Json::objectValue:
  	  return decode_object (jval);
  	  break;
  }
  return octave_value ("<unimplented JSON type>");
}

octave_value
decode_json_text (std::string json_str) {
  Json::CharReaderBuilder builder;
  Json::Value root;
  std::istringstream istr (json_str);
  JSONCPP_STRING errs;
  bool ok = Json::parseFromStream (builder, istr, &root, &errs);
  if (ok) {
    return decode_recursive (root);
  } else {
    // TODO: Throw Octave error
    return octave_value ("<JSON decoding error>");
  }
}


DEFUN_DLD (__jsonstuff_jsondecode_oct__, args, nargout,
  "Decode JSON text to Octave value\n"
  "\n"
  "-*- texinfo -*-\n"
  "@deftypefn {Function File} {@var{out} =} __oct_time_binsearch__ (@var{json_text})\n"
  "\n"
  "Undocumented internal function for jsonstuff package.\n"
  "\n"
  "@end deftypefn\n")
{
  int nargin = args.length ();
  if (nargin != 1) {
    std::cout << "Error: Invalid number of arguments. Expected 1; got "
      << nargin << "\n";
    return octave_value_list ();  // TODO: raise Octave error
  }

  octave_value json_text_ov = args(0);
  builtin_type_t json_text_ov_type = json_text_ov.builtin_type ();
  if (json_text_ov_type != btyp_char) {
    std::cout << "Error: unsupported input data type: expected char, got "
      << json_text_ov_type << "\n";
    return octave_value_list (); // TODO: raise Octave error
  }
  std::string json_str = json_text_ov.string_value ();

  octave_value decoded_value = decode_json_text (json_str);

  return decoded_value;
}