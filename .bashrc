# .bashrc
LANG=ko_KR.UTF-8
PS1="\[\u@\w/ \!\$ \]"
# SecureCRT sw Options > Session Options > Appearance : Character : UTF-8
# ▒▒▒▒ؾ▒ ▒▒▒▒ ▒▒▒▒.
#LANG=en_US
history=100
savehist=50
alias   bks='stty erase '
alias   m=more
alias   h=history
alias   df='df -k'
alias   w=who
alias   dir='ls -l'
alias   ll='ls -al'
alias   l='ls -l'
alias   lf='ls -F'
alias   lt='ls -alt | more'
alias   lm='ls -l | more'
#alias   llm='ls -al \!:$ | more'
alias   pa='ps -eaf | more'
#alias   pu='pushd \!* && cd .'
#alias   po='popd \!* && cd .'
