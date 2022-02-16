import("lib.nas");
var char_ttf=[
    ["    ","    ","    ","    ","    ","    "],
    ["    ████╗","   ██╔██║","  ██╔╝██║"," ███████║","██╔═══██║","╚═╝   ╚═╝"],
    ["██████╗ ","██╔══██╗","██████╔╝","██╔══██╗","██████╔╝","╚═════╝ "],
    [" ██████╗","██╔════╝","██║     ","██║     ","╚██████╗"," ╚═════╝"],
    ["██████╗ ","██╔══██╗","██║  ██║","██║  ██║","██████╔╝","╚═════╝ "],
    ["███████╗","██╔════╝","█████╗  ","██╔══╝  ","███████╗","╚══════╝"],
    ["███████╗","██╔════╝","█████╗  ","██╔══╝  ","██║     ","╚═╝     "],
    [" █████╗ ","██╔═══╝ ","██║ ██╗ ","██║ ╚██╗","╚█████╔╝"," ╚════╝ "],
    ["██╗  ██╗","██║  ██║","███████║","██╔══██║","██║  ██║","╚═╝  ╚═╝"],
    [" ██████╗","   ██╔═╝","   ██║  ","   ██║  "," ██████╗"," ╚═════╝"],
    ["     ██╗","     ██║","     ██║","██   ██║","╚█████╔╝"," ╚════╝ "],
    ["██╗  ██╗","██║ ██╔╝","█████╔╝ ","██╔═██╗ ","██║  ██╗","╚═╝  ╚═╝"],
    ["██╗     ","██║     ","██║     ","██║     ","███████╗","╚══════╝"],
    ["██╗   ██╗","███╗ ███║","████████║","██╔██╔██║","██║╚═╝██║","╚═╝   ╚═╝"],
    ["██╗   ██╗","███╗  ██║","█████╗██║","██╔█████║","██║ ╚███║","╚═╝  ╚══╝"],
    [" ██████╗ ","██╔═══██╗","██║   ██║","██║   ██║","╚██████╔╝"," ╚═════╝ "],
    ["██████╗ ","██╔══██╗","██████╔╝","██╔═══╝ ","██║     ","╚═╝     "],
    [" ██████╗ ","██╔═══██╗","██║   ██║","██║  ██╔╝","╚████╔██╗"," ╚═══╝╚═╝"],
    ["██████╗ ","██╔══██╗","██████╔╝","██╔══██╗","██║  ██║","╚═╝  ╚═╝"],
    ["███████╗","██╔════╝","███████╗","╚════██║","███████║","╚══════╝"],
    ["████████╗","╚══██╔══╝","   ██║   ","   ██║   ","   ██║   ","   ╚═╝   "],
    ["██╗   ██╗","██║   ██║","██║   ██║","██║   ██║","╚██████╔╝"," ╚═════╝ "],
    ["██╗   ██╗","██║   ██║","██║   ██║","╚██╗ ██╔╝"," ╚████╔╝ ","  ╚═══╝  "],
    ["██╗   ██╗","██║██╗██║","████████║","███╔═███║","██╔╝ ╚██║","╚═╝   ╚═╝"],
    ["██╗  ██╗","╚██╗██╔╝"," ╚███╔╝ "," ██╔██╗ ","██╔╝╚██╗","╚═╝  ╚═╝"],
    ["██╗   ██╗","╚██╗ ██╔╝"," ╚████╔╝ ","  ╚██╔╝  ","   ██║   ","   ╚═╝   "],
    ["████████╗","╚════██╔╝","   ██╔═╝ "," ██╔═╝   ","████████╗","╚═══════╝"],
];
var trans_ttf=func(string)
{
    var str=["","","","","",""];
    for(var i=0;i<size(string);i+=1)
    {
        var number=string[i];
        if(97<=number and number<=122)
            for(var j=0;j<6;j+=1)
                str[j]~=char_ttf[number-96][j];
        elsif(65<=number and number<=90)
            for(var j=0;j<6;j+=1)
                str[j]~=char_ttf[number-64][j];
        else
            for(var j=0;j<6;j+=1)
                str[j]~=char_ttf[0][j];
    }
    foreach(var i;str)
        println(i);
    return;
}
var curve1=func()
{
    var shadow=["░","▒","▓","█","▀","▄","▐","▌"];
    rand(100);
    var s="";
    for(var i=0;i<10;i+=1)
    {
        for(var j=0;j<40;j+=1)
            s~=shadow[int(8*rand())];
        s~='\n';
    }
    print(s);
}
var curve2=func()
{
    var table=["╚","═","╝","╔","║","╗"];
    rand(100);
    var s="";
    for(var i=0;i<10;i+=1)
    {
        for(var j=0;j<40;j+=1)
            s~=table[int(6*rand())];
        s~='\n';
    }
    print(s);
}
var curve3=func()
{
    var s=["","","","","",""];
    var cnt=0;
    foreach(var char;char_ttf)
    {
        cnt+=1;
        forindex(var i;char)
            s[i]~=char[i];
        if(cnt==9)
        {
            forindex(var i;s)
            {
                println(s[i]);
                s[i]='';
            }
            cnt=0;
        }
    }
    return;
}
var curve4=func()
{
    var arr=[0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8];
    for(var loop=0;loop<10;loop+=1)
    {
        for(var i=26;i>=0;i-=1)
        {
            var rand_index=int(i*rand());
            (arr[i],arr[rand_index])=(arr[rand_index],arr[i]);
        }
        #            0   1   2   3   4   5   6   7   8
        var shadow=[" ","░","▒","▓","█","▀","▄","▐","▌"];
        var s="";
        for(var i=0;i<size(arr);i+=1)
                s~=shadow[arr[i]];
        println(s);
    }
    return;
}
trans_ttf("just for test");
trans_ttf(" ValKmjolnir ");
trans_ttf("just for fun");
curve1();
curve2();
curve3();
curve4();