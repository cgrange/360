app.directive('flipShiz', ['$timeout', function ($timeout) {
    return {
        link: function ($scope, element, attrs) {
            $scope.$on('dataloaded', function () {
                $timeout(function () { // You might need this timeout to be sure its run after DOM render.
                    $('.flipper').flip();
                }, 0, false);
            })
        }
    };
}]);
