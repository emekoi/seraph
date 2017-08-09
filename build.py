#!/usr/bin/python2.7
import os, sys, shutil, platform, time

OUTPUT = "bin/seraph"
COMPILER = "gcc"
INCLUDE = [  ]
SOURCE = [
  "src/*.c",
  "src/lib/sera/*.c",
]

FLAGS = [ "-Wall", "-Wextra", "--std=c99", "-fno-strict-aliasing", "-Wno-unused-function", "-Wno-misleading-indentation", "-Wno-unused-parameter" ]
LINK = [ "m" ]
DEFINE = [ ]
EXTRA = ""

if platform.system() == "Windows":
  sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
  OUTPUT += ".exe"
  LINK += [ "mingw32", "SDL2main", "SDL2" ]
  FLAGS += [ "-mwindows" ]

if platform.system() == "Linux":
  LINK += [ "SDL2main", "SDL2" ]

if platform.system() == "Darwin":
  LINK += [  ]
  FLAGS += [  ]
  FLAGS += [ os.popen("sdl2-config --cflags").read().strip() ]
  EXTRA += os.popen("sdl2-config --libs").read().strip()
  DEFINE += [ "SR_MODE_ARGB" ]


def fmt(fmt, dic):
  for k in dic:
    fmt = fmt.replace("{" + k + "}", str(dic[k]))
  return fmt


def main():
  global FLAGS, SOURCE, LINK

  print "initing..."
  starttime = time.time()

  # Handle args
  build = "debug" if "debug" in sys.argv else "release"
  verbose = "verbose" in sys.argv

  # Handle build type
  if build == "debug":
    FLAGS += [ "-g" ]
  else:
    FLAGS += [ "-O3" ]

  print "building (" + build + ")..."

  # Create directories
  outdir = os.path.dirname(OUTPUT)
  if not os.path.exists(outdir):
    os.makedirs(outdir)

  # Build
  cmd = fmt(
    "{compiler} -o {output} {flags} {source} {include} {link} {define} " +
    "{extra}",
    {
      "compiler"  : COMPILER,
      "output"    : OUTPUT,
      "source"    : " ".join(SOURCE),
      "include"   : " ".join(map(lambda x:"-I" + x, INCLUDE)),
      "link"      : " ".join(map(lambda x:"-l" + x, LINK)),
      "define"    : " ".join(map(lambda x:"-D" + x, DEFINE)),
      "flags"     : " ".join(FLAGS),
      "extra"     : EXTRA,
    })

  if verbose:
    print cmd

  print "compiling..."
  res = os.system(cmd)

  if build == "release":
    if os.path.isfile(os.path.dirname(OUTPUT)):
      print "stripping..."
      os.system("strip %s" % OUTPUT)

  if res == 0:
    print "done (%.2fs)" % (time.time() - starttime)
  else:
    print "done with errors"
  sys.exit(res)


if __name__ == "__main__":
  main()
