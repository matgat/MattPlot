    /* * * * * * * * * * * * * * * * * * * * * * * * * * *\
    *
    *   `hlp_scripts.css´ v0.02 2007 Matteo Gattanini
    *
    *   Some shared `Javascript´ code
    *
    \* * * * * * * * * * * * * * * * * * * * * * * * * * */


//============================= Global variables ============================


//================================ Functions ================================

//---------------------------------------------------------------------------
function doSection (sectID)
{ // Display the section if it's not displayed; hide it if it is displayed
    if ( sectID.style.display=="none" ) sectID.style.display="";
    else sectID.style.display="none";
}
function noSection (sectID)
{ // Remove the section when user clicks in the opened div
    if ( sectID.style.display=="" ) sectID.style.display="none";
}

//---------------------------------------------------------------------------
function liteOn (objID)
{
    objID.style.background="#cc0033";
    objID.style.color="#FFFFFF";
}
function liteOff (objID)
{
    objID.style.background="transparent";
    objID.style.color="#000000";
}



//---------------------------------------------------------------------------
function PrintPage()
{ // Print the page
    window.print(); 
    // In document:
    // document.write("<form><input type=button value=\"Print\" onClick=\"PrintPage()\"></form>");
}



//---------------------------------------------------------------------------
function PrintDateTime ()
{ // Get date and time
    now = new Date();

    //off = now.getTimeZoneOffset();
    //gmt = now.toGMTString();
    //day = now.getDay();
    //mon = now.getMonth();
    //year = now.getYear(); //getFullYear();
    //date = now.getDate();
    //hour = now.getHours();
    //min = now.getMinutes();
    //sec = now.getSeconds();
    //time = now.getTime();
    //time = ((hour<=9)?"0"+hour:hour) +":"+((min<=9)?"0"+min:min);//+":"+((sec<=9)?"0"+sec:sec);

    document.title = now.toLocaleString();
    setTimeout("PrintDateTime()", 1000);
}

//---------------------------------------------------------------------------
//function popupLink(url,w,h)
{ // Open a popup explorer window
  // Use:   <a href="Javascript:popupLink('xxx.htm',600,500)">here</a>
    window.open(url,'Scrap','toolbar=no,width='+w+',height='+h+',resizable,scrollbars,status=no');
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



//=============================== End of file ===============================
