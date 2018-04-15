// Javascript graph creation

	data = "http://109.246.189.57:32000/data.csv"; // path to CSV file bypassing the browser cache

	g1 = new Dygraph(
    document.getElementById("temperature"),
    data,
    {
			title: 'Temperature',
            ylabel: 'Temperature (&#176;C)',
			xlabel: 'Date',
			axes: {
				y: {
				valueFormatter: function(v) {
				return v + '&#176;C';  // controls formatting in the legend/mouseover
				}}},
			valueRange: [0, 30.1],
			showRangeSelector: true,
			visibility: [true,false,false,false],
			titleHeight: 45,
			xLabelHeight: 30,
			color: 'red',
			rangeSelectorPlotFillColor: "red",
			rangeSelectorBackgroundStrokeColor : "",
			rangeSelectorPlotFillGradientColor: "",
			rangeSelectorPlotStrokeColor: "",
			rangeSelectorHeight: 20
	}          // options
  );
  
   g2 = new Dygraph(
    document.getElementById("pressure"),
    data,
    {
			title: 'Pressure',
            ylabel: 'Pressure (mbar)',
			xlabel: 'Date',
			axes: {
				y: {
				valueFormatter: function(v) {
				return v + 'mbar';  // controls formatting in the legend/mouseover
				}}},
			valueRange: [940, 1060.5],
            showRangeSelector: true,
			visibility: [false,false,true,false],
			titleHeight: 45,
			xLabelHeight: 30,
			color: 'green',
			rangeSelectorPlotFillColor: "green",
			rangeSelectorBackgroundStrokeColor : "",
			rangeSelectorPlotFillGradientColor: "",
			rangeSelectorPlotStrokeColor: "",
			rangeSelectorHeight: 20
	
	}          // options
  );
  
     g3 = new Dygraph(
    document.getElementById("humidity"),
    data,
    {
			title: 'Humidity',
            ylabel: 'Relative Humidity (%)',
			xlabel: 'Date',
			axes: {
				y: {
				valueFormatter: function(v) {
				return v + '%';  // controls formatting in the legend/mouseover
				}}},	
			valueRange: [0, 100.5],			
            showRangeSelector: true,
			visibility: [false,true,false,false],
			titleHeight: 45,
			xLabelHeight: 30,
			color: 'blue',
			rangeSelectorPlotFillColor: "blue",
			rangeSelectorBackgroundStrokeColor : "",
			rangeSelectorPlotFillGradientColor: "",
			rangeSelectorPlotStrokeColor: "",
			rangeSelectorHeight: 20
	
	}          // options
  );
  
     g4 = new Dygraph(
    document.getElementById("brightness"),
    data,
    {
			title: 'Brightness',
            ylabel: 'Brightness (lux)',
			xlabel: 'Date',
			axes: {
				y: {
				valueFormatter: function(v) {
				return v + 'lux';  // controls formatting in the legend/mouseover
				}}},	
			valueRange: [0, 1000.5],	
            showRangeSelector: true,
			visibility: [false,false,false,true],
			titleHeight: 45,
			xLabelHeight: 30,
			color: 'orange',
			rangeSelectorPlotFillColor: "orange",
			rangeSelectorBackgroundStrokeColor : "",
			rangeSelectorPlotFillGradientColor: "",
			rangeSelectorPlotStrokeColor: "",
			rangeSelectorHeight: 20
	
	}          // options
  );
  

  
function openGraph(evt, tabName) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    evt.currentTarget.className += " active";
	g1.resize(750,350);
	g2.resize(750,350);
	g3.resize(750,350);
	g4.resize(750,350);
}

// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();

