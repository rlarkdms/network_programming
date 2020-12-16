#
umask 022
setenv  SHELL   /bin/bash
set     path = ($HOME/bin /bin /sbin /usr/bin /usr/sbin /etc /usr/etc \
                /usr/ucb /usr/X11R6/bin /usr/local/bin /usr/lib )
#set    ignoreeof noglob
#set    tmp=(`tset -s -Q`)
#set    mail=/usr/spool/mail
#setenv MAIL /usr/spool/mail
#setenv MAILCHECK 600
#setenv MAILMSG 'You Have New Mail.'
#if ( -s $MAIL ) then      # This is at Shell startup.  In normal
#if ( !~ -z $MAIL ) echo $MAILMSG      # This is at Shell startup.  In normal
#if (-s $MAIL ) echo $MAILMSG          # operation, the Shell checks
#endif                     # periodically.
#NOT WORK ??????
#setenv TERM    $tmp[1]
#unset  tmp noglob
#
#set TERMTYPE=`tty | grep hft`
#if [ -n "$TERMTYPE" -o -n "$XSTATION" ]
#then
#       if [ -f /usr/lpp/X11/bin/xinit ]
#       then
#               /usr/lpp/X11/bin/xinit
#       fi
#fi
#
who
