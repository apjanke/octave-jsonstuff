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

#include <octave/oct.h>
#include "json/json.h"

// Inputs:
//   1: Encoded JSON data as char vector
// Outputs:
//   1: Decoded value as Octave value

class decode_result {
public:
  octave_value value;
  bool is_condensed;
  decode_result (const NDArray &val)
    : value (octave_value (val)), is_condensed (false) {}
  decode_result (const octave_value &val)
    : value (val), is_condensed (false) {}
  decode_result (const octave_value &val, bool condensed)
    : value (val), is_condensed (condensed) {}
};

decode_result
decode_recursive (const Json::Value &jval);

decode_result
decode_double (const Json::Value &jval) {
  NDArray out (dim_vector (1, 1));
  out(0) = jval.asDouble ();
  return out;
}

bool
equals (const string_vector &a, const string_vector &b) {
  if (a.numel () != b.numel ()) {
    return false;
  }
  octave_idx_type n = a.numel ();
  for (octave_idx_type i = 0; i < n; i++) {
    if (a(i) != b(i)) {
      return false;
    }
  }
  return true;
}

decode_result
decode_array (const Json::Value &jval) {
  // Decode all the elements first, and then decide how to combine them,
  // based on the set of element types.
  // All JSON numerics are effectively doubles, so we can just ignore the Int/UInt stuff.
  bool is_all_numeric = true;
  for (int i = 0; i < jval.size (); i++) {
  	Json::Value v = jval[i];
  	if (! (v.isNumeric () || v.isNull ())) {
      is_all_numeric = false;
      break;
  	}
  }
  if (is_all_numeric) {
    if (jval.empty ()) {
      // Special case: empty numerics are [], not 1-by-0
      return NDArray (dim_vector (0, 0));
    } else {
    	NDArray out (dim_vector (jval.size (), 1));
    	for (int i = 0; i < jval.size (); i++) {
    	  if (jval[i].isNull ()) {
    	  	out(i) = NAN;
    	  } else {
    	    out(i) = jval[i].asDouble ();
    	  }
    	}
    	return out;
    }
  } else {
    bool is_any_child_condensed = false;
    bool is_all_child_structs = true;
    int n_children = jval.size();
  	Cell children (dim_vector (n_children, 1));
  	for (int i = 0; i < n_children; i++) {
  	  auto rslt = decode_recursive (jval[i]);
  	  is_all_child_structs &= rslt.value.isstruct ();
  	  is_any_child_condensed |= rslt.is_condensed;
  	  children(i) = rslt.value;
  	}
  	bool is_maybe_condensable = is_all_child_structs && ! is_any_child_condensed;
  	if (! is_maybe_condensable) {
      return octave_value (children);
    }
    bool is_condensable = true;
    octave_map s0 = children(0).map_value ();
    string_vector fields0 = s0.fieldnames ();
    for (int i_child = 0; i_child < n_children; i_child++) {
      octave_map s = children(i_child).map_value ();
      string_vector fields = s.fieldnames ();
      // For some reason, empty structs are not considered condensable
      if (fields.numel() == 0) {
        is_condensable = false;
        break;
      }
      if (! (equals (fields0, fields))) {
        is_condensable = false;
        break;
      }
    }
    if (is_condensable) {
      octave_map s (dim_vector (n_children, 1));
      for (octave_idx_type i = 0; i < n_children; i++) {
        s.assign (idx_vector (i), children(i).map_value ());
      }
      return decode_result (s, true);
    } else {
      return octave_value (children);
    }
  }
}

decode_result
decode_object (const Json::Value &jval) {
  octave_scalar_map s;
  Json::Value::Members members = jval.getMemberNames ();
  for (size_t i = 0; i < members.size (); i++) {
    auto rslt = decode_recursive (jval[members[i]]);
  	s.assign (members[i], rslt.value);
  }
  return octave_value (s);
}

decode_result
decode_string (const Json::Value &jval) {
  return octave_value (jval.asString ());
}

decode_result
decode_boolean (const Json::Value &jval) {
  boolNDArray out = boolNDArray (dim_vector (1, 1));
  out(0) = jval.asBool ();
  return octave_value (out);
}

decode_result
decode_null (const Json::Value &jval) {
  NDArray out (dim_vector (0, 0));
  return out;
}

decode_result
decode_recursive (const Json::Value &jval) {
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
  // This shouldn't happen
  error ("Internal error: Unimplemented JSON type");
}

decode_result
decode_json_text (const std::string &json_str) {
  Json::CharReaderBuilder builder;
  Json::Value root;
  std::istringstream istr (json_str);
  JSONCPP_STRING errs;
  bool ok = Json::parseFromStream (builder, istr, &root, &errs);
  if (ok) {
    return decode_recursive (root);
  } else {
    error ("Invalid JSON data");
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
  octave_idx_type nargin = args.length ();
  if (nargin != 1) {
    error ("Invalid number of arguments: expected 1; got %ld", (long) nargin);
  }

  octave_value json_text_ov = args(0);
  builtin_type_t json_text_ov_type = json_text_ov.builtin_type ();
  if (json_text_ov_type != btyp_char) {
    error ("Error: unsupported input data type: expected char");
  }

  std::string json_str = json_text_ov.string_value ();
  decode_result decoded = decode_json_text (json_str);
  return decoded.value;
}