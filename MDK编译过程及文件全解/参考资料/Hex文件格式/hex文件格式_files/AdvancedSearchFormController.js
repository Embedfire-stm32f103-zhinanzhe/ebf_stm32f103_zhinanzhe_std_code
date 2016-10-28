"use strict";

keilapp.controller("AdvancedSearchFormController", function ($scope, $location) {
    // Q: Why not use $location.search()?
    // A: At the time of writing, keilapp was more or less global and since you 
    //    cannot nest apps, it wasn't practical to configure a $locationProvider 
    //    for keilapp because of the potential impact upon the rest of the system. 
    //    Also since we need to support HTML5 and hashbang it was easier to just 
    //    parse the url in order to get query keys and values.
    $scope.getQueryParamValue = function (key) {

        var result = '';

        var absUrl = $location.absUrl();
        // Remove plus character (encoded or decoded)
        absUrl = absUrl.replace(/\+/g, ' ').replace(/%2B/g, ' ');

        key = key.toLowerCase();

        // Does a param key and value exist?
        if (absUrl.toLowerCase().indexOf(key + "=") > -1 &&
            (absUrl.toLowerCase().indexOf(key + "=&") == -1)) {

            // Key and values exist so get the rhs of the url
            var rhs = absUrl.substring(absUrl.toLowerCase().indexOf(key + "="));

            // Double check for a value
            if (rhs.indexOf("&") > -1) {
                result = rhs.substring(rhs.indexOf("=") + 1, rhs.indexOf("&"));
            }
        }

        return result;
    }
    // Search term is the user specified search term (look for previous value in the url).
    $scope.searchTerm = $scope.getQueryParamValue("searchTerm");
    // Product family filter value (look for previous value in the url).
    $scope.productFamily = $scope.getQueryParamValue("productFamily");
    // Modified days filter value (look for previous value in the url).
    $scope.modifiedDays = $scope.getQueryParamValue("modifiedDays");
    // The site to use is determined by the url.
    $scope.getSite = function () {

        // First look for a query param to default to
        var site = null;
        var index;
        var sites = ["keilsupportdocs", "keilsupportman", "keilappnotes", "keildd", "keildownload", "keilforum", "keil"];
        for (index = 0; index < sites.length; index++) {
            if ($scope.getQueryParamValue("site") === sites[index]) {
                site = sites[index];
            }
        }

        // If there wasn't an existing site query param then set a 
        // default site based upon the url.
        if (site === null) {
            var absUrl = $location.absUrl();

            if (absUrl.indexOf("support\/docs") > -1) {
                site = "keilsupportdocs";
            }
            else if (absUrl.indexOf("support\/man") > -1) {
                site = "keilsupportman";
            }
            else if (absUrl.indexOf("appnotes") > -1) {
                site = "keilappnotes";
            }
            else if (absUrl.indexOf("dd") > -1) {
                site = "keildd";
            }
            else if (absUrl.indexOf("download") > -1) {
                site = "keildownload";
            }
            else if (absUrl.indexOf("forum") > -1) {
                site = "keilforum";
            }
            else { site = "keil"; }
        }

        return site;
    }
    // Initially show advanced options if there was a value specified.
    $scope.advancedOptions = ($scope.productFamily != '' || $scope.modifiedDays != '');
    // Toggle between show or hide advanced options.
    $scope.toggleAdvancedOptions = function () {
        $scope.advancedOptions = !$scope.advancedOptions;
    }
    // Toggle advanced options button should show expand or collapse.
    $scope.advancedOptionsBtnText = $scope.advancedOptions ? '-' : '+';
    // Watch for changes
    $scope.$watch(function () {
        return $scope.advancedOptions;
    }, function (newValue, oldValue) {
        $scope.advancedOptionsBtnText = newValue ? '-' : '+';
    });
});