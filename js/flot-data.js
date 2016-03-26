//Flot Line Chart
$(document).ready(function() {
    console.log("document ready");
    var offset = 0;
    plot();

    function plot() {

       var options = {
            series: {
                lines: {
                    show: true
                },
                points: {
                    show: true
                }
            },
            xaxes: [{
                mode: 'time'
            }],
            yaxes: [{
                min: 0.0
            }],
            legend: {
                position: 'sw'
            },
            grid: {
                hoverable: true //IMPORTANT! this is needed for tooltip to work
            },
            tooltip: true,
            tooltipOpts: {
                content: "%s was %y",
                xDateFormat: "%y-%0m-%0d",
                onHover: function(flotItem, $tooltipEl) {
                    // console.log(flotItem, $tooltipEl);
                }
            }


        };


      
        $.getJSON("http://192.168.1.38/index.php/main/getSensorData/temp_sensor", function (data) {
            sensor_data = []
            for ( var i =0 ; i < data.data.length; i++ ) {
                sensor_data.push([data.data[i][0], parseFloat(data.data[i][1])]);
            }

            var plotObj = $.plot($("#flot-line-chart-temperature"), [{
                    data: sensor_data,
                    label: data.label 
                }
                ],
                options);
         });
       
        $.getJSON("http://192.168.1.38/index.php/main/getSensorData/ph_sensor", function (data) {
            sensor_data = []
            for ( var i =0 ; i < data.data.length; i++ ) {
                sensor_data.push([data.data[i][0], parseFloat(data.data[i][1])]);
            }

            var plotObj = $.plot($("#flot-line-chart-ph"), [{
                    data: sensor_data,
                    label: data.label 
                }
                ],
                options);
         });
 
 
        $.getJSON("http://192.168.1.38/index.php/main/getSensorData/uv_sensor", function (data) {
            sensor_data = []
            for ( var i =0 ; i < data.data.length; i++ ) {
                sensor_data.push([data.data[i][0], parseFloat(data.data[i][1])]);
            }

            var plotObj = $.plot($("#flot-line-chart-uv"), [{
                    data: sensor_data,
                    label: data.label 
                }
                ],
                options);
         });
 




    }



});

