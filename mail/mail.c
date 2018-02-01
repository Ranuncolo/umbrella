#include <stdio.h>
#include "quickmail.h"

/*
    This file requires libquickmail and libcurl libraries.

    Both libquickmail and libcurl are free softwares that can be redistributed
    and/or modified under the terms of the GNU General Public License as
    published by the Free Software Foundation.
*/

#define FROM        "my.email@ydomain.com"
#define TO          "your.email@domain.com"
#define CC          "other@domain.com"  // you can comment it
#define BCC         "other@domain.com"  // you can comment it
#define SMTPSERVER  "smtp.server.com"   // define the smtp server
#define SMTPPORT    1                   // define the smtp port
#define SMTPUSER    "my.email@domain.com"
#define SMTPPASS    "mypassword"

void list_attachment_callback (quickmail mailobj, const char* filename, quickmail_attachment_open_fn email_info_attachment_open, quickmail_attachment_read_fn email_info_attachment_read, quickmail_attachment_close_fn email_info_attachment_close, void* callbackdata)
{
  printf("[%i]: %s\n", ++*(int*)callbackdata, filename);
}

//------------------------------------------------------------------------------

int main ()
{
  printf("libquickmail %s\n", quickmail_get_version());
  quickmail_initialize();
  quickmail mailobj = quickmail_create(FROM, "subject"); // modify your subject
#ifdef TO
  quickmail_add_to(mailobj, TO);
#endif
#ifdef CC
  quickmail_add_cc(mailobj, CC);
#endif
#ifdef BCC
  quickmail_add_bcc(mailobj, BCC);
#endif
  quickmail_add_header(mailobj, "Importance: Low");
  quickmail_add_header(mailobj, "X-Priority: 5");
  quickmail_add_header(mailobj, "X-MSMail-Priority: Low");
  quickmail_set_body(mailobj, "Your text");
  quickmail_add_body_memory(mailobj, "text/html", "your text in html", 80, 0);
  quickmail_add_attachment_file(mailobj, "attachment", NULL);
  // can add more attachment

  const char* errmsg;
  quickmail_set_debug_log(mailobj, stderr);
  if ((errmsg = quickmail_send(mailobj, SMTPSERVER, SMTPPORT, SMTPUSER, SMTPPASS)) != NULL)
    fprintf(stderr, "Error sending e-mail: %s\n", errmsg);
  quickmail_destroy(mailobj);
  quickmail_cleanup();
  return 0;
}
