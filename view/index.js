function init_controller( cli, obj ) {

    cli.send( "hello world!" );

    requestAnimationFrame((ts)=>{ 
        init_controller( cli )
    });

}

(()=>{
    
    var cli = new WebSocket( "wss://192.168.1.146:8000" ); // 192.168.1.146
    var enb = false; var obj ={ x:"0", y:"0", z:"0" };

    window.addEventListener("deviceorientation", (ev)=>{
        obj.x = ev.alpha.toFixed(2);
        obj.y = ev.beta .toFixed(2);
        obj.z = ev.gamma.toFixed(2);
    if( enb ){ cli.send( JSON.stringify(obj) ); }}, true);

    cli.onopen = ()=>{ enb=true; }

    cli.onmessage = (data)=>{ 
        console.log( "->", data );
    }

    cli.onclose = ()=>{
        console.log( "disconnected" );
        enb = false;
    }

})();