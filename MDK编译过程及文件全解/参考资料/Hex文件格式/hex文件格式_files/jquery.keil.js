
$(document).ready(function () {
    // cookie functions
    // based on http://www.quirksmode.org/js/cookies.html

    // Create a test cookie to see if the user has cookies enabled
    create('testCookies', 'yes', 365, '/', '.keil.com');
    // Delete old cookie
    create("acceptCookies", '', -1, '/', '.keil.com');
    //read acceptCookie to get the cookie value set
    var cVal = read("acceptCookie");

    if ((read("testCookies") == 'yes') && (!cVal)) {
        jQuery('#cookiemodule').css('display', 'block');

        jQuery('button.accept').click(function () {
            create('acceptCookie', '3', 365, '/', '.keil.com');
            jQuery('#cookiemodule').fadeOut('slow');
        });

        jQuery('button.close').click(function () {
            jQuery('#cookiemodule').fadeOut('slow');
        });
    }
});

function create(name, value, days, path, domain) {
    path = typeof path !== 'undefined' ? path : '/';
    domain = typeof domain !== 'undefined' ? '; domain=' + domain : '';
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        var expires = "; expires=" + date.toGMTString();
    } else {
        var expires = "";
    }
    document.cookie = name + "=" + value + expires + domain + "; path=" + path;
}
function read(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') c = c.substring(1, c.length);
        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length);
    }
    return null;
}

function erase(name) {
    create(name, "", -1);
}

// Erase cookies by category
function eraseCookies(category) {

    switch (category) {
        case '2':
            var i = 0;
            var cookieArray = ['__unam', '__utma', '__utmb', '__utmc', '__utmd', '__utmz', '_mkto_trk', 'user', 'search', 'fm_accept', 'fm', 'eval', 'disti'];
            for (i = 0; i < cookieArray.length; i++) {
                create(cookieArray[i], '', -1, '/', '.keil.com');
            }
            cookieArray.length = 0;
            break;
        case '3':
            var i = 0;
            var cookieArray = ['_mkto_trk'];
            for (i = 0; i < cookieArray.length; i++) {
                create(cookieArray[i], '', -1, '/', '.keil.com');
            }
            cookieArray.length = 0;
            break;
    }
}

// Pass through category and save to cookie
function saveCookies(category) {
    //create a cookie whose value is the number above.
    create('acceptCookie', category, 365, '/', '.keil.com');
    //erase cookies not relevant to this section
    eraseCookies(category);
}