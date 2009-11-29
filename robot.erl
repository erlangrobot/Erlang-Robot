-module(robot).
-export([start/1, sleep_infinite/0, read_ir/0, move_forward/0]).
-export([out_gate/1, read_replies/1]).

start(Args) ->
   spawn(robot, move_forward, []),
   spawn(robot, read_ir, []),
   sleep_infinite().

sleep_infinite() -> 
	receive 
	after infinity -> 
		true
	end.


out_gate(Port) ->
   receive
	{K,output} -> 
	    Port ! {self(), {command, [49,K]}};
	{Port,{data,Any}} ->
                read ! read_ir
   end,
   out_gate(Port).


move_forward() -> 
   register(move,self()),
   Port = open_port({spawn,port},[{packet,2}]),
   out_gate(Port).

read_replies(Port) ->
	receive
	    read_ir -> 
   		Port ! {self(), {command, [50]}}; 
	         
	    {Port, {data, Any}} ->
		if 
		  Any == [97] -> move ! {9,output};
		  Any == [98] -> move ! {18,output};
		  Any == [99] -> move ! {10,output};
		  Any == [100] -> move ! {0,output};
		  Any == [101] -> move ! {17,output}
		end	
	end,
	read_replies(Port).

read_ir() ->
	register(read,self()),
	Port = open_port({spawn,port},[{packet,2}]),
	Port ! {self(),{command, [50]}},
	read_replies(Port).
