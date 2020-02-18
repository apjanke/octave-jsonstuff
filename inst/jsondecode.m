## Copyright (C) 2019 Andrew Janke <floss@apjanke.net>
##
## This file is part of Octave.
##
## Octave is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {Function} {@var{value} =} jsondecode (@var{text})
## @deftypefnx {Function} {@var{value} =} jsondecode (@var{text})
##
## Encode Octave data as JSON.
##
## Encodes the Octave value @code{data} in JSON format and returns the
## result as a character vector.
##
## @code{jsonencode(..., "ConvertInfAndNaN", TF)} controls the encoding of special floating
## point values NaN, Inf, and -Inf.
##
## @xref{jsondecode}
##
## @end deftypefn

function out = jsondecode (text)
  data = __jsonstuff_jsondecode_oct__ (text);
  out = condense_decoded_json (data);
endfunction

function out = condense_decoded_json (x)
  out = condense_decoded_json_recursive (x);
endfunction

function out = condense_decoded_json_recursive (x)
  if ~iscell (x)
    out = x;
    return
  endif
  if isempty (x)
    out = x;
    return
  endif
  x2 = x;

  % I _think_ this is the proper condensation for a 1-long array?
  if isscalar (x2) && isnumeric (x2{1})
    out = x2{1};
    return
  end

  % Depth-first: condense the component arrays first
  for i = 1:numel (x2)
    x2{i} = condense_decoded_json_recursive (x{i});
  endfor

  % Numeric arrays are condensable if they have all the same dimensions
  % All other condensation has been handled by the oct-file
  sz = size (x2{1});
  is_condensable = true;
  for i = 1:numel (x2)
    if ~isnumeric (x2{i})
      is_condensable = false;
      break
    endif
    if ~isequal (size (x2{i}), sz)
      is_condensable = false;
      break
    endif
  endfor

  if is_condensable
    n_dims = ndims(x2{1});
    ix_permute = [n_dims+1 1:n_dims];
    for i = 1:numel (x2)
      x2{i} = permute (x2{i}, ix_permute);
    endfor
    out = cat (1, x2{:});
  else
    out = x2;
  endif

endfunction

%!assert (jsondecode ('42'), 42)
%!assert (jsondecode ('"foobar"'), "foobar")
%!assert (jsondecode ('null'), [])
%!assert (jsondecode ('[]'), [])
%!assert (jsondecode ('[1, 2, 3]'), [1; 2; 3])
%!assert (jsondecode ('[1, 2, null]'), [1; 2; NaN])
%!assert (jsondecode ('[1, 2, "foo"]'), {1; 2; "foo"})
%!assert (jsondecode ('{"foo": 42, "bar": "hello"}'), struct("foo",42, "bar","hello"))
%!assert (jsondecode ('[{"foo": 42, "bar": "hello"}, {"foo": 1.23, "bar": "world"}]'), struct("foo", {42; 1.23}, "bar", {"hello"; "world"}))
%!assert (jsondecode ('[[1, 2], [3, 4]]'), [1 2; 3 4])
%!assert (jsondecode ('[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]'), cat(3, [1 3; 5 7], [2 4; 6 8]))