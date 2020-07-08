var keilapp = angular.module('keilapp', ['ngSanitize', 'angularGrid', 'ui.bootstrap']);

//$(".nav a").on("click", function () {
//    $(".nav").find(".active").removeClass("active");
//    $(this).parent().addClass("active");
//});

//$(".nav a").on("click", function () {
//    $(".nav").find(".active").removeClass("active");
//    $(this).addClass("active");
//    $(this).closest('a.dropdown-toggle').addClass("active");
    
//});
$(function () {
    var currenthref = window.location.href;
    var page = currenthref.substr(currenthref.lastIndexOf('/') + 1);
    page = "/adm/" + page;
    if (page) {
        $("#admmenu").find(".active").removeClass("active");
        $('#admmenu li a[href="' + page + '"]').closest('li').addClass('active');
        $('#admmenu li a[href="' + page + '"]').closest('li.dropdown').addClass('active')
    }
 
})