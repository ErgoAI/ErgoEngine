
ERGO="../../../ErgoAI/runergo --nobanner --noprompt --nofeedback --quietload "
#---------------------------
#1 000 000
../../../ErgoAI/runergo --nobanner --noprompt --nofeedback --quietload \
		-e "chatter{off},[bench_tc_chain],%bench_chain(1000000,1),\halt."

#10 000 000
$ERGO -e "chatter{off},[bench_tc_chain],%bench_chain(10000000,1),\halt."

#100 000 000
$ERGO -e "chatter{off},[bench_tc_chain],%bench_chain(100000000,1),\halt."

#---------------------------
# Cycle
#1 000 000
$ERGO -e "chatter{off},[bench_tc_cycle],%bench_cycle(1000000,1),\halt."

#10 000 000
$ERGO -e "chatter{off},[bench_tc_cycle],%bench_cycle(10000000,1),\halt."

#100 000 000
$ERGO -e "chatter{off},[bench_tc_cycle],%bench_cycle(100000000,1),\halt."

#---------------------------
# Self-loop
#1 000 000
$ERGO -e "chatter{off},[bench_tc_self_loop],%bench_self_loop(1000000,1),\halt."

#10 000 000
$ERGO -e "chatter{off},[bench_tc_self_loop],%bench_self_loop(10000000,1),\halt."

#100 000 000
$ERGO -e "chatter{off},[bench_tc_self_loop],%bench_self_loop(100000000,1),\halt."

#---------------------------
# Frame chain
#1 000 000
$ERGO -e "chatter{off},[bench_tc_frame_chain],%bench_frame_chain(1000000,1),\halt."

#10 000 000
$ERGO -e "chatter{off},[bench_tc_frame_chain],%bench_frame_chain(10000000,1),\halt."

#100 000 000
$ERGO -e "chatter{off},[bench_tc_frame_chain],%bench_frame_chain(100000000,1),\halt."

#---------------------------
# Frame cycle
#1 000 000
$ERGO -e "chatter{off},[bench_tc_frame_cycle],%bench_frame_cycle(1000000,1),\halt."

#10 000 000
$ERGO -e "chatter{off},[bench_tc_frame_cycle],%bench_frame_cycle(10000000,1),\halt."

#100 000 000
$ERGO -e "chatter{off},[bench_tc_frame_cycle],%bench_frame_cycle(100000000,1),\halt."

#---------------------------
# Frame self-loop
#1 000 000
$ERGO -e "chatter{off},[bench_tc_frame_self_loop],%bench_frame_self_loop(1000000,1),\halt."

#10 000 000
$ERGO -e "chatter{off},[bench_tc_frame_self_loop],%bench_frame_self_loop(10000000,1),\halt."

#100 000 000
$ERGO -e "chatter{off},[bench_tc_frame_self_loop],%bench_frame_self_loop(100000000,1),\halt."

#---------------------------
