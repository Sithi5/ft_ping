# Test -c valid host

ping -c 1 8.8.8.8 > ping.log
sudo ./ft_ping -c 1 8.8.8.8 > ft_ping.log
diff ping.log ft_ping.log

rm ping.log ft_ping.log