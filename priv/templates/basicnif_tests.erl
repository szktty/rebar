-module({{modid}}_tests).

-include_lib("eunit/include/eunit.hrl").

new_test() ->
    Bin = <<1,2,3,4,5>>,
    Context = {{modid}}:new(Bin),
    ?assertEqual(Bin, {{modid}}:get(Context)).

