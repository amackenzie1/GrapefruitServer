function BuildDateString()
{
    var today = new Date()
    var year = today.getYear()
    var _day = today.getDate()
    if (_day < 10)
        _day = "0" + _day
    var _month = today.getMonth() + 1
    if (_month < 10)
        _month = "0" + _month
    var hours = today.getHours()
    var minutes = today.getMinutes()
    var seconds = today.getSeconds()
    var dn = "AM" 
    if (hours > 12)
    {
        dn = "PM"
        hours = hours - 12
    }
    if (hours == 0)
        hours = 12
    if (minutes < 10)
        minutes = "0" + minutes
    if (seconds < 10)
        seconds = "0" + seconds
    var DateString = _month+"/"+_day+"/"+year+" "+hours+":"+minutes+":"+seconds+" "+dn

    return DateString;
}
console.log(BuildDateString())
