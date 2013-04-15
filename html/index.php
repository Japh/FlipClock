<html>
	<head>
		<title>Flip Clock</title>
		<link rel="stylesheet" href="css/style.css" />
		<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
		<script src="js/prototype.js"></script>
	</head>

	<body>
		<div id="clockface">
			<div class="col1 row1 slot1 top-slot"></div>
			<div class="col2 row1 slot2 top-slot"></div>
			<div class="col3 row1 slot3 top-slot"></div>
			<div class="col1 row2 slot4 top-slot"></div>
			<div class="col2 row2 slot5 top-slot"></div>
			<div class="col3 row2 slot6 top-slot"></div>
			<div class="col4 row2 slot7 top-slot"></div>

			<div class="col1 row1 slot1 bottom-slot"></div>
			<div class="col2 row1 slot2 bottom-slot"></div>
			<div class="col3 row1 slot3 bottom-slot"></div>
			<div class="col1 row2 slot4 bottom-slot"></div>
			<div class="col2 row2 slot5 bottom-slot"></div>
			<div class="col3 row2 slot6 bottom-slot"></div>
			<div class="col4 row2 slot7 bottom-slot"></div>
		</div>
		<div id="preloader">
			<?php
			for ( $i = 0; $i < 10; $i++ ) {
				echo '<img src="../resources/src/images/num_' . $i . '.png" />' . "\n";
			}
			$months = array(
				'jan',
				'feb',
				'mar',
				'apr',
				'may',
				'jun',
				'jul',
				'aug',
				'sep',
				'oct',
				'nov',
				'dec',
			);
			foreach ( $months as $month ) {
				echo '<img src="../resources/src/images/month_' . $month . '.png" />' . "\n";
			}
			?>
		</div>
	</body>
</html>
