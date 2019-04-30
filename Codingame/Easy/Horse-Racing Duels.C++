function sortHorses(a , b ) {
    return a-b;
}
const n = readline();
if( n < 2 ) {
    print (readline());
}
else
{
    var horses = [];
    
    for (var i = 0; i < n; i++) {
       horses.push(parseInt(readline()));  
    }
    horses.sort(sortHorses);
    printErr(horses);
    var smallestDiff = horses[1] - horses[0];
    for(var i = 2 ; i < n; i++ )
    {
        var newDiff =  horses[i] - horses[ i - 1];
        if( newDiff < smallestDiff )
        {
            smallestDiff  = newDiff;
        } 
    }
    print ( smallestDiff );
}
