#include "defines.h"
#include "kozos.h"
#include "consdrv.h"
#include "lib.h"

/* コンソール・ドライバの使用開始をコンソール・ドライバに依頼する */
static void send_use(int index)
{
  char *p;
  p = kz_kmalloc(3);
  p[0] = '0';
  p[1] = CONSDRV_CMD_USE;
  p[2] = '0' + index;
  kz_send(MSGBOX_ID_CONSOUTPUT, 3, p);
}

/* コンソールへの文字列出力をコンソール・ドライバに依頼する */
static void send_write(char *str)
{
  char *p;
  int len;
  len = strlen(str);
  p = kz_kmalloc(len + 3);
  p[0] = '0';
  p[1] = CONSDRV_CMD_WRITE;
  memcpy(&p[2], str, len + 1);
  kz_send(MSGBOX_ID_CONSOUTPUT, len + 2, p);
}

int command_main(int argc, char *argv[])
{
  char *p;
  int size;

  send_use(SERIAL_DEFAULT_DEVICE);

  while (1) {
    send_write("command> "); /* プロンプト表示 */

    /* コンソールからの受信文字列を受け取る */
    kz_recv(MSGBOX_ID_CONSINPUT, &size, &p);
    p[size] = '\0';

    if (!strncmp(p, "echo", 4)) { /* echoコマンド */
      send_write(p + 4); /* echoに続く文字列を出力する */
      send_write("\n");
    } else if(!strncmp(p, "lua", 3)){
    	void *L = luaL_newstate();
    	#define LUA_MULTRET (-1)
    	#define luaL_dostring(L,s) \
    		(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
    	#define lua_pcall(L,n,r,f)      lua_pcallk(L, (n), (r), (f), 0, NULL)
    	luaL_openlibs(L);
    	luaL_dostring(L, p + 4);
    } else {
      send_write("unknown.\n");
    }

    kz_kmfree(p);
  }

  return 0;
}
