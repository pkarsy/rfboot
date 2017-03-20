import posix

{.compile: "serial.c".}
proc openport*(port: cstring):cint {.importc.}
proc readser*(fd: cint, buf: cstring, nbytes: cint, timeout: cint): cint {.importc.}

proc getChar(port: cint, timeout: cint = 100000): int =
  #var c: char
  var buf: array[4,char];
  #let status = port.spBlockingRead(addr c,1,timeout)
  let n = readser(port, buf, 1, timeout)
  if n < 0:
    stderr.writeLine "Erron : ", n, " while reading serial port"
    quit QuitFailure
  elif n==0: return -1
  else: return buf[0].int

let fd = openport("/dev/ttyACM0")
#echo "fd=", fd
var buf: array[4,char];
#var buf: cstring = "1234"

let b = readser(fd, buf, 3, 500000)
#echo b
if b>0:
  echo buf

