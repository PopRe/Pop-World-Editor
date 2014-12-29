
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#define LOG_FILE   "popedt.log"
#define LOG_TITLE  "<--- Populous World Editor --->"
#define LOG_INFO   "Compilation: %s, %s\r\nVersion: %s"
#define LOG_TIME   "Time: %0.2d/%0.2d/%0.4d %0.2d:%0.2d:%0.2d\r\n"
#define LOG_END    "<--- End Log --->"



long LogCreate();
long LogClose();
long LogWrite(char *str);
