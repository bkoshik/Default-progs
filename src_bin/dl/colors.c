#include "modules.h"

Permission color_permission() {
  Permission p;
  p.read = DarkGreen;
  p.write = DarkYellow;
  p.exec = DarkRed;
  p.exec_sticky = AnsiValue_5;
  p.no_access = Grey;
  p.octal = AnsiValue_6;
  p.acl = DarkCyan;
  p.context = Cyan;

  p.file = AnsiValue_8;
  p.dir = AnsiValue_34;
  p.symlink = AnsiValue_44;
  p.pipe = AnsiValue_44;         // darkturquoise
  p.block_device = AnsiValue_44; // darkturquoise
  p.char_device = AnsiValue_172; // orange3
  p.socket = AnsiValue_44;       // darkturquoise
  p.special = AnsiValue_44;      // darkturquoise
  return p;
}

User_Group color_user_group() {
  User_Group ug;
  ug.user = AnsiValue_230;
  ug.group = AnsiValue_93;
  return ug;
}
