function __run_tests_and_exit__ (pkg_name)
  pkg('load', pkg_name);
  pkg_info = pkg('list', pkg_name);
  pkg_dir = pkg_info{1}.dir;
  nfailed = my_runtests (pkg_dir);
  fprintf ('Number of failed tests: %d\n', nfailed);
  exit (double (nfailed > 0));
endfunction

## Copyright (C) 2010-2018 John W. Eaton
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
## @deftypefn  {} {} runtests ()
## @deftypefnx {} {} runtests (@var{directory})
## Execute built-in tests for all m-files in the specified @var{directory}.
##
## Test blocks in any C++ source files (@file{*.cc}) will also be executed
## for use with dynamically linked oct-file functions.
##
## If no directory is specified, operate on all directories in Octave's search
## path for functions.
## @seealso{rundemos, test, path}
## @end deftypefn

## Author: jwe

function nfailed = my_runtests (directory)

  if (nargin == 0)
    dirs = ostrsplit (path (), pathsep ());
    do_class_dirs = true;
  elseif (nargin == 1)
    dirs = {canonicalize_file_name(directory)};
    if (isempty (dirs{1}) || ! isdir (dirs{1}))
      ## Search for directory name in path
      if (directory(end) == '/' || directory(end) == '\')
        directory(end) = [];
      endif
      fullname = dir_in_loadpath (directory);
      if (isempty (fullname))
        error ("runtests: DIRECTORY argument must be a valid pathname");
      endif
      dirs = {fullname};
    endif
    do_class_dirs = false;
  else
    print_usage ();
  endif

  nfailed = 0;
  for i = 1:numel (dirs)
    d = dirs{i};
    nfailed += run_all_tests (d, do_class_dirs);
  endfor

endfunction

function nfailed_total = run_all_tests (directory, do_class_dirs)

  nfailed_total = 0;
  flist = readdir (directory);
  dirs = {};
  no_tests = {};
  printf ("Processing files in %s:\n\n", directory);
  fflush (stdout);
  for i = 1:numel (flist)
    f = flist{i};
    if ((length (f) > 2 && strcmpi (f((end-1):end), ".m"))
        || (length (f) > 3 && strcmpi (f((end-2):end), ".cc")))
      ff = fullfile (directory, f);
      if (has_tests (ff))
        print_test_file_name (f);
        [p, n, xf, xb, sk, rtsk, rgrs] = test (ff, "quiet");
        nfailed = n - p - xf - xb - rgrs;
        nfailed_total += nfailed;
        print_pass_fail (p, n, xf, xb, sk, rtsk, rgrs);
        fflush (stdout);
      elseif (has_functions (ff))
        no_tests(end+1) = f;
      endif
    elseif (f(1) == "@")
      f = fullfile (directory, f);
      if (isdir (f))
        dirs(end+1) = f;
      endif
    endif
  endfor
  if (! isempty (no_tests))
    printf ("\nThe following files in %s have no tests:\n\n", directory);
    printf ("%s", list_in_columns (no_tests));
  endif

  ## Recurse into class directories since they are implied in the path
  if (do_class_dirs)
    for i = 1:numel (dirs)
      d = dirs{i};
      nfailed_total += run_all_tests (d, false);
    endfor
  endif

endfunction


function retval = has_functions (f)

  n = length (f);
  if (n > 3 && strcmpi (f((end-2):end), ".cc"))
    fid = fopen (f);
    if (fid < 0)
      error ("runtests: fopen failed: %s", f);
    endif
    str = fread (fid, "*char")';
    fclose (fid);
    retval = ! isempty (regexp (str,'^(?:DEFUN|DEFUN_DLD|DEFUNX)\>',
                                    'lineanchors', 'once'));
  elseif (n > 2 && strcmpi (f((end-1):end), ".m"))
    retval = true;
  else
    retval = false;
  endif

endfunction

function retval = has_tests (f)

  fid = fopen (f);
  if (fid < 0)
    error ("runtests: fopen failed: %s", f);
  endif

  str = fread (fid, "*char").';
  fclose (fid);
  retval = ! isempty (regexp (str,
                              '^%!(assert|error|fail|test|xtest|warning)',
                              'lineanchors', 'once'));

endfunction

function print_pass_fail (p, n, xf, xb, sk, rtsk, rgrs)

  if ((n + sk + rtsk + rgrs) > 0)
    printf (" PASS   %4d/%-4d", p, n);
    nfail = n - p - xf - xb - rgrs;
    if (nfail > 0)
      printf ("\n%71s %3d", "FAIL ", nfail);
    endif
    if (rgrs > 0)
      printf ("\n%71s %3d", "REGRESSION", rgrs);
    endif
    if (xb > 0)
      printf ("\n%71s %3d", "(reported bug) XFAIL", xb);
    endif
    if (xf > 0)
      printf ("\n%71s %3d", "(expected failure) XFAIL", xf);
    endif
    if (sk > 0)
      printf ("\n%71s %3d", "(missing feature) SKIP", sk);
    endif
    if (rtsk > 0)
      printf ("\n%71s %3d", "(run-time condition) SKIP", rtsk);
    endif
  endif
  puts ("\n");

endfunction

function print_test_file_name (nm)
  filler = repmat (".", 1, 60-length (nm));
  printf ("  %s %s", nm, filler);
endfunction


%!error runtests ("foo", 1)
%!error <DIRECTORY argument> runtests ("#_TOTALLY_/_INVALID_/_PATHNAME_#")
