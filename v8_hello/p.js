

setTimeout(function(){
	var a = 1;
	print("in timer");
},5000);



var c = 0;
var timerId = setInterval(function(){
	c++;
	var t = timerId;
	print("[" + t + "]cnt:" + c);
	if( c > 5 ){
		clearInterval(t);
	}
}, 2000);

print("debuger");
Inject;
