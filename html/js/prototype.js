(function( $ ) {
	"use strict";

	$(function() {

		var months = new Array('jan','feb','mar','apr','may','jun','jul','aug','sep','oct','nov','dec');

		$('#clockface div').click(function (e) {

			//console.log( e.target );

			var $slot = $(e.target);
			var classes = $slot.attr('class').split(' ');

			var $slot1_top = $('.' + classes[2] + '.top-slot');
			var $slot1_bottom = $('.' + classes[2] + '.bottom-slot');

			var $new_top = $('<div />');
			for (var i = 0; i < classes.length; i++) {
				$new_top.addClass(classes[i]);
			}
			$new_top.removeClass('bottom-slot');
			$new_top.addClass('top-slot');

			var $new_bottom = $('<div />');
			for (var i = 0; i < classes.length; i++) {
				$new_bottom.addClass(classes[i]);
			}
			$new_bottom.removeClass('top-slot');
			$new_bottom.addClass('bottom-slot');

			//console.log( $new_top );
			//console.log( $new_bottom );

			var $current_number = $slot.css('background-image');
			$current_number = $current_number.split('/');
			$current_number = $current_number[$current_number.length - 1];
			$current_number = $current_number.replace(/\..*/, '');
			$current_number = $current_number.split('_');
			//console.log( $current_number );

			if ( $current_number[0] === 'num' ) {
				$current_number[1]++;
			}
			else if ( $current_number[0] === 'month' ) {
				$current_number[1] = months[months.indexOf($current_number[1]) + 1];
			}

			//console.log( $current_number );

			var new_image_url = '../resources/src/images/' + $current_number.join('_') + '.png';
			//console.log( new_image_url );
			$new_top.css('background-image', 'url('+new_image_url+')');
			$new_bottom.css('background-image', 'url('+new_image_url+')');

			$new_top.css('z-index', 0);
			$new_bottom.css('z-index', 999);
			$new_bottom.css('backgroundPositionY', "-84px");

			$('#clockface').append( $new_top );
			$('#clockface').append( $new_bottom );

			$slot1_top.animate({
				backgroundPositionY: "42px"
			}, 75, 'linear', function() {
				$new_bottom.animate({
					backgroundPositionY: "-42px"
				}, 75, 'linear', function() {
					$slot1_top.remove();
					$slot1_bottom.remove();
				});
			});

		});

	});

}(jQuery));
