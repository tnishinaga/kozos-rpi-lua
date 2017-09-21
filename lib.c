#include "defines.h"
#include "serial.h"
#include "lib.h"

void *memset(void *b, int c, long len)
{
  char *p;
  for (p = b; len > 0; len--)
    *(p++) = c;
  return b;
}

void *memcpy(void *dst, const void *src, long len)
{
  char *d = dst;
  const char *s = src;
  for (; len > 0; len--)
    *(d++) = *(s++);
  return dst;
}

int memcmp(const void *b1, const void *b2, long len)
{
  const char *p1 = b1, *p2 = b2;
  for (; len > 0; len--) {
    if (*p1 != *p2)
      return (*p1 > *p2) ? 1 : -1;
    p1++;
    p2++;
  }
  return 0;
}

int strlen(const char *s)
{
  int len;
  for (len = 0; *s; s++, len++)
    ;
  return len;
}

char *strcpy(char *dst, const char *src)
{
  char *d = dst;
  for (;; dst++, src++) {
    *dst = *src;
    if (!*src) break;
  }
  return d;
}

int strcmp(const char *s1, const char *s2)
{
  while (*s1 || *s2) {
    if (*s1 != *s2)
      return (*s1 > *s2) ? 1 : -1;
    s1++;
    s2++;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, int len)
{
  while ((*s1 || *s2) && (len > 0)) {
    if (*s1 != *s2)
      return (*s1 > *s2) ? 1 : -1;
    s1++;
    s2++;
    len--;
  }
  return 0;
}

/* １文字送信 */
int putc(unsigned char c)
{
  if (c == '\n')
    serial_send_byte(SERIAL_DEFAULT_DEVICE, '\r');
  return serial_send_byte(SERIAL_DEFAULT_DEVICE, c);
}

/* １文字受信 */
unsigned char getc(void)
{
  unsigned char c = serial_recv_byte(SERIAL_DEFAULT_DEVICE);
  c = (c == '\r') ? '\n' : c;
  putc(c); /* エコー・バック */
  return c;
}

/* 文字列送信 */
int puts(char *str)
{
  while (*str)
    putc(*(str++));
  return 0;
}

/* 文字列受信 */
int gets(char *buf)
{
  int i = 0;
  unsigned char c;
  do {
    c = getc();
    if (c == '\n')
      c = '\0';
    buf[i++] = c;
  } while (c);
  return i - 1;
}

/* 数値の16進表示 */
int putxval(unsigned long value, int column)
{
  char buf[9];
  char *p;

  p = buf + sizeof(buf) - 1;
  *(p--) = '\0';

  if (!value && !column)
    column++;

  while (value || column) {
    *(p--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    if (column) column--;
  }

  puts(p + 1);

  return 0;
}

int _exit()
{
  while (1)
    ;
  return 0;
}

int _fstat() { return 0; }
int _open() { return 0; }

extern char _heap;
char *_heap_tail = &_heap;

void *_sbrk(int incr)
{
  char *p = _heap_tail;
  _heap_tail += incr;
  return (void *)p;
}
int _kill() { return 0; }
int _getpid() { return 0; }
int _times() { return 0; }
int _unlink() { return 0; }
int _write(int fd, char *buf, int size)
{
  int i = 0;
  while (size > i) {
    while (!serial_is_send_enable(0))
      ;
    serial_send_byte(0, buf[i++]);
  }
  return i;
}
int _close() { return 0; }
int _gettimeofday() { return 0; }
int _isatty()
{
  return 1;
}
int _link() { return 0; }
int _lseek() { return 0; }
int _read(int fd, char *buf, int size)
{
  int i = 0;
  while (size > 0) {
    while (!serial_is_recv_enable(0))
      ;
    buf[i++] = serial_recv_byte(0);
    if (buf[i - 1] == '\n')
      break;
  }
  return i;
}
