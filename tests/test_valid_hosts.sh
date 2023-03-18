
_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_CYAN=`tput setaf 3`
_END=`tput sgr0`

# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"


# Test -c valid host 8.8.8.8
echo "\n${_GREEN}Test -c valid host 8.8.8.8 :${_END}\n"

ping -c 1 8.8.8.8 > ping.log
sudo ./ft_ping -c 1 8.8.8.8 > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log

# Test -c valid host  with domain name google.com
echo "\n\n${_GREEN}Test -c valid host google.com :${_END}\n"

ping -c 1 google.com > ping.log
sudo ./ft_ping -c 1 google.com > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log