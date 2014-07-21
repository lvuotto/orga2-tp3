
buf = ''

with open('task1.tsko', 'rb') as f:
  c = f.read(1)
  while c != '':
    buf += c
    c = f.read(1)

with open('task1.tsko', 'wb') as f:
  f.write(chr(0xe9))
  f.write(str(4096 - len(buf)))
  for i in range(5, 4096 - len(buf)):
    f.write(chr(0))
  f.write(buf)
