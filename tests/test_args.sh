
_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_CYAN=`tput setaf 3`
_END=`tput sgr0`

# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"


# Test args -c 1 with valid host
echo "\n${_GREEN}Test args -c 1 with valid host :${_END}\n"

ping -c 1 8.8.8.8 > ping.log
sudo ./ft_ping -c 1 8.8.8.8 > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log

# Test invalid args before host
echo "\n\n${_GREEN}Test invalid args before host :${_END}\n"

ping --invalidargs google.com > ping.log
sudo ./ft_ping --invalidargs google.com > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log

# Test invalid args after host
echo "\n\n${_GREEN}Test invalid args after host :${_END}\n"

ping  google.com --invalidargs> ping.log
sudo ./ft_ping google.com --invalidargs > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log

# Test no args
echo "\n\n${_GREEN}Test no args :${_END}\n"

ping > ping.log
sudo ./ft_ping > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log


# Test -c with invalid number after
echo "\n\n${_GREEN}Test -c with invalid number after :${_END}\n"

ping  -c 8.8.8.8 8.8.8.8 > ping.log
sudo ./ft_ping -c 8.8.8.8 8.8.8.8 > ft_ping.log

echo "\n${_CYAN}ping:${_END}\n"
cat ping.log
echo "\n${_CYAN}ft_ping:${_END}\n"
cat ft_ping.log

rm ping.log ft_ping.log
