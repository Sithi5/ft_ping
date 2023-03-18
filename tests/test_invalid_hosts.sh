
_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_CYAN=`tput setaf 3`
_END=`tput sgr0`

# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"


# Test -c invalid host invalid domain name
echo "\n${_GREEN}Test -c invalid host invalidhost :${_END}\n"

ping -c 1 invalidhost > ping.log
sudo ./ft_ping -c 1 invalidhost > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log

# Test -c invalid host invalid ip address
echo "\n${_GREEN}Test -c invalid host invalid ip address :${_END}\n"

ping -c 1 123.123.123.123 > ping.log
sudo ./ft_ping -c 1 123.123.123.123 > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log
