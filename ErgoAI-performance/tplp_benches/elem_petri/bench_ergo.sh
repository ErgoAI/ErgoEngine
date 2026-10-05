ERGO="../../../ErgoAI/runergo --nobanner --noprompt --nofeedback --quietload "
#---------------------------
#100 000
$ERGO -e "chatter{off},[bnergo],generate_net(100000),%bench_elem_net,\halt."

#200 000
$ERGO -e "chatter{off},[bnergo],generate_net(200000),%bench_elem_net,\halt."

#500 000
$ERGO -e "chatter{off},[bnergo],generate_net(500000),%bench_elem_net,\halt."

#1 000 000
$ERGO -e "chatter{off},[bnergo],generate_net(1000000),%bench_elem_net,\halt."

#2 000 000
$ERGO -e "chatter{off},[bnergo],generate_net(2000000),%bench_elem_net,\halt."

