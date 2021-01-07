
/*
*  ##########################################################  SOCKETS  ##########################################################
*/

var socket_connect_display= true;
var socket_display        = true;
var web_socket            = null;
var socket_connected      = false;
var socket_keep_alive_period = 500;
var keep_alive_enabled = false;
var json_data;
						
function socket_init(){
	console.log("socket_init");
	web_socket = new WebSocket('ws://'+document.location.host+'/ws',['arduino']);
	web_socket.binaryType = "arraybuffer";
	web_socket.debug = true;
	console.log('ws://'+document.location.host+'/ws\n');

	web_socket.onopen = function(e) { 
		if (socket_connect_display) console.log("ws open",    e); 
		socket_connected=true;
		socket_status_display();


		if (keep_alive_enabled) keep_alive_time_out=setTimeout(socket_keep_alive, socket_keep_alive_period);
	};
	 
	web_socket.onclose = function(e){
		if (socket_connect_display) console.log("ws close", e); 
		socket_connected=false;
		socket_status_display();
		// setTimeout(function() {  socket_init()  }, 1000);
	};
	 
	web_socket.onerror = function(e){
		if (socket_connect_display) console.log("ws error",      e); 
		socket_connected=false;
		// setTimeout(function() {  socket_init()  }, 1000);
		socket_status_display();
	};
	 
	web_socket.onmessage = function(e){
		var msg = "";
		if (e.data instanceof ArrayBuffer){
			msg = "BIN:";
			var bytes = new Uint8Array(e.data);
			for (var i = 0; i < bytes.length; i++) {
				msg += String.fromCharCode(bytes[i]);
			}
		} else {
			msg = e.data;
		}
		socket_receive(msg);
	};
}

function socket_keep_alive() {
	if (!web_socket) return;
	if (web_socket.readyState !== 1) return;
	// client_request("socket_keep_alive");
	keep_alive_time_out=setTimeout(socket_keep_alive, socket_keep_alive_period);
}

function socket_receive(msg) {
			// if (socket_display) console.log("socket msg received: "+msg);
			server_response(msg);
}

function socket_send(msg) {
	if (! socket_connected ) socket_init();
		web_socket.send(msg); 
		if (socket_display) console.log("socket msg sent: "+msg);
}



function socket_status_display() {
	// if (socket_connected) document.getElementById('lbl_upd').value = "connected";
	// else document.getElementById('lbl_upd').value = "disconnected";
}


// VAR
var response_display = true;

var table_isCreated = false;

var first_start = true;


// SOCKET REQUEST RECIEVE
function server_response(json) {
	if (json == "{\"}") return;
	if (response_display) { console.log('json:'); console.log(json); console.log('');}

	var response = JSON.parse(json);
	if (response_display) { console.log('response:'); console.log(response);}

	if (response.l_g != null) cmb_set_group(response.l_g);

	if (response.l_o != null) {

		if (table_isCreated == false) {
			table_create(response.l_o);
			if (first_start){
				first_start = false;
				table_upd(response.l_o);
			}
			table_isCreated = true;
		} else {
			table_upd(response.l_o);
		}
	}
}

// REQUEST UTILE
function literal_item(name, value) {
   var x="<"+name+">"+value+"</"+name+">";
   return x;
}


// SOCKET REQUEST SEND 
	//TABLE
	function table_request_outputSelect(argument) {
		var argument2 	= document.getElementById("ol_select_" + argument).textContent;
		console.log("table_request_outputSelect: " + argument2);

		var dn  		= document.getElementById("ol_dn_" + argument).textContent;
		var name  		= document.getElementById("ol_ol_" + argument).textContent;
		var postion  	= document.getElementById("ol_op_" + argument).textContent;

		console.log("dn: " + dn + "- name: " + name + " - cmd: " + argument2  +  " - position: " + postion);

		var cmd = "&";

		if (argument2 == "On") {
			cmd += "s_output=" + postion + "|0";
		}
		if (argument2 == "Off") {
			cmd += "s_output=" + postion + "|1";
		}

		var s_dn 	= literal_item("dn", 	dn);
		var msg 	= literal_item("msg", 	cmd);
		var req 	= s_dn + msg;

		console.log("\treq: " + req);

		socket_send(req);
	}	
	function table_request_onoff(argument, argument2) {
		var dn  		= document.getElementById("ol_dn_" + argument).textContent;
		var name  		= document.getElementById("ol_ol_" + argument).textContent;
		var postion  	= document.getElementById("ol_op_" + argument).textContent;

		console.log("dn: " + dn + "- name: " + name + " - cmd: " + argument2  +  " - position: " + postion);

		var cmd = "&";

		if (argument2 == "on") {
			cmd += "ss_lp=" + postion + "|0";
		}
		if (argument2 == "off") {
			cmd += "ss_lp=" + postion + "|1";
		}

		var s_dn 	= literal_item("dn", 	dn);
		var msg 	= literal_item("msg", 	cmd);
		var req 	= s_dn + msg;

		console.log("\treq: " + req);

		socket_send(req);
	}
	function table_request_colorPicker(val){
		var picker 		= document.getElementById("ol_cp_" + val).style.backgroundColor
		var rgb 		= rgbToComponents(picker); 
		var dn  		= document.getElementById("ol_dn_" + val).textContent;
		var name  		= document.getElementById("ol_ol_" + val).textContent;
		var postion  	= document.getElementById("ol_op_" + val).textContent;

		console.log("dn: " + dn + "- name: " + name +   " - position: " + postion);
		console.log("\tR: " + rgb.r + " - G: " + rgb.g + " - B: " + rgb.b );

		var s_dn 	= literal_item("dn",		dn);
		var msg 	= literal_item("msg",		"&ss_lp_rgb=" + postion + "|" + rgb.r + "." +  rgb.g + "." +  rgb.b);
		// var s_pos 	= literal_item("postion",	postion);
		// var s_val 	= literal_item("rgb", 		rgb.r + "." +  rgb.g + "." +  rgb.b);
		var req 	= s_dn + msg;

		console.log("\treq: " + req);

		socket_send(req);

	}
	function table_request_hBri(val){
		var bri 		= document.getElementById("ol_sli_hbri" + val).value;
		var dn  		= document.getElementById("ol_dn_" + val).textContent;
		var name  		= document.getElementById("ol_ol_" + val).textContent;
		var postion  	= document.getElementById("ol_op_" + val).textContent;
		var tw  		= document.getElementById("ol_tw_" + val).textContent;

		console.log("dn: " + dn + "- name: " + name +   " - position: " + postion);
		console.log("\tbri: " + bri );

		var s_dn = literal_item("dn", dn);
		var msg ;
		if (tw == 0) 	msg = literal_item("msg",		"&ss_led_bri=" 	+ postion + "|" + bri);
		else			msg = literal_item("msg",		"&ss_lp_white=" + postion + "|" + bri);
		// var s_pos 	= literal_item("postion",	postion);
		// var s_val 	= literal_item("rgb", 		rgb.r + "." +  rgb.g + "." +  rgb.b);
		var req 	= s_dn + msg;

		console.log("\treq: " + req);

		socket_send(req);

	}

// TABLE MANAGMENT
function table_upd_row() { 
	var style = "";
	var s_grp = document.getElementById("input_group").value;
	if ( s_grp == "") return;
	var count = $('#points-table tr').length;
	var gName;
	for(var j = 0; j < count-1; j++) {
			gName = document.getElementById("ol_grp_"+j).textContent;
			if (s_grp == "all") {
				style = "";
			} else {
				if (s_grp == gName) {
					style = "";
				} else {
					style = "none";
				}					
			}
			document.getElementById("ol_dn_"+j).style.display = style; 
			document.getElementById("ol_grp_"+j).style.display = style; 
			document.getElementById("ol_ol_"+j).style.display = style; 
			document.getElementById("ol_os_"+j).style.display = style; 
			document.getElementById("ol_oc_"+j).style.display = style; 
			document.getElementById("ol_cp_"+j).style.display = style; 
			document.getElementById("tset_grp_"+j).style.display = style; 
			document.getElementById("ol_on_"+j).style.display = style; 
			document.getElementById("ol_off_"+j).style.display = style; 
			document.getElementById("ol_sli_hbri"+j).style.display = style; 
			document.getElementById("ol_check_"+j).style.display = style; 
			document.getElementById("ol_tr_"+j).style.display = style; 
	}  

}
function table_get_itemPos(dn, ol, op ){
	var count = $('#points-table tr').length;
	var s_dn;
	var s_ol;
	var s_op;
	var ret = -1;
	for(var j = 0; j < count-1; j++) {
			s_dn = document.getElementById("ol_dn_"+j).textContent;
			s_ol = document.getElementById("ol_ol_"+j).textContent;
			s_op = document.getElementById("ol_op_"+j).textContent;
			if ((s_dn == dn) && (s_ol == ol) && (s_op == op) )  {
				ret = j;

			}
	}
	return ret;
}
function table_upd(chaine) {

	// console.log("\ndevice_list\n");

	var val;
	var tableau2;
	var tableau3;

	//hide col
	document.getElementById("tableID_ip").style.display = "none"; 
	document.getElementById("tableID_pin").style.display = "none"; 
	document.getElementById("tableID_tw").style.display = "none"; 

	var count = 0;

	chaine = "" + chaine + "";

	// device
	var tableau     = chaine.split("|");
	for(var i = 0; i < tableau.length-1; i++) {

		// outputs
		tableau2 = tableau[i].split("*");
		console.log("test: " + tableau2[2] + "\n");
		for(var j = 2; j < tableau2.length-1; j++) {


			tableau3 = tableau2[j].split(";");

			count = table_get_itemPos(tableau2[0], tableau3[0], tableau3[3]);
			if (count == -1) continue;
			// hide items
			document.getElementById("ol_ip_"+count).style.display = "none"; 
			document.getElementById("ol_op_"+count).style.display = "none"; 
			document.getElementById("ol_tw_"+count).style.display = "none"; 

			var v1 = document.getElementById("ol_dn_"+count).textContent ;
			var v2 = document.getElementById("ol_ol_"+count).textContent ;
			var v3 = document.getElementById("ol_os_"+count).textContent ;
			var v4 = document.getElementById("ol_op_"+count).textContent ;

			// console.log("\t" + v1 + " = " + v2 + " = " + v3 + "\n")
			// console.log("\t" + tableau2[0] + " = " + tableau3[0] + " = " + tableau3[2] + "\n\n" )


			if (( tableau2[0] == v1 ) && ( tableau3[0] == v2 ) && ( tableau3[3] == v4 )){

				if (tableau3[2] == "1") {
					document.getElementById("ol_os_"+count).innerHTML =  "On";
					document.getElementById("ol_os_" + count).style.color = "rgb(0,255,0)";
				} else {
					document.getElementById("ol_os_"+count).innerHTML =  "Off";
					document.getElementById("ol_os_" + count).style.color = "rgb(255,0,0)";
				}
				if (tableau3[8] == "1") {
					document.getElementById("ol_select_"+count).innerHTML =  "On";
				} else {
					document.getElementById("ol_select_"+count).innerHTML =  "Off";
				}

				var rgb = tableau3[4].split(".");
	            var bgColor = "rgb(" + rgb[0] + "," + rgb[1] + "," + rgb[2] + ")";            
	            document.getElementById('ol_cp_'+count).style.backgroundColor = bgColor; 

				document.getElementById("ol_oc_"+count).innerHTML =  tableau3[7];
				document.getElementById("ol_tw_"+count).innerHTML =  tableau3[6];
			}

			// count++;
		}        
	}
}

function table_check_all() {
	var style = document.getElementById("customCheck1").checked;;
	var count = $('#points-table tr').length;
	for(var j = 0; j < count-1; j++) {

			var display = document.getElementById("ol_dn_"+j).style.display;
			if (display == "none") continue;

			document.getElementById("dl_check_"+j).checked = style;


	}  	
}
function table_create(chaine) {

		// console.log("\ndevice_list\n");

		var val;
		var tableau2;
		var tableau3;
		var  html="";
			html+="<thead>";
			html+="    <tr>";
			html+="        <th >";
							html+="<div class=\"custom-control custom-checkbox\">";
							html+="<input onchange=\"table_check_all()\" type=\"checkbox\" class=\"custom-control-input\" id=\"customCheck1\" >";
							html+="<label class=\"custom-control-label\" for=\"customCheck1\"></label>";
							html+="</div>";
			html+="        </th>";
			html+="        <th>device</th>";
			html+="        <th id=tableID_ip style=\"display:\">ip</th>";
			html+="        <th>grp</th>";
			html+="        <th>location</th>";
			html+="        <th id=tableID_pin>pin postion</th>";
			html+="        <th>state</th>";
			html+="        <th>online</th>";
			html+="        <th style=\"width:250px\">select</th>";
			html+="        <th>cmd on</th>";
			html+="        <th>cmd off</th>";
			html+="        <th>colorpicker</th>";
			html+="        <th>Hbri</th>";
			html+="        <th id=tableID_tw>truewhite</th>";
			html+="    </tr>";
			html+="</thead>";

			html+="<tbody>\n"; 

		var count = 0;

		chaine = "" + chaine + "";

		// device
		var tableau = chaine.split("|");

		for(var i = 0; i < tableau.length-1; i++) {

			tableau2 = tableau[i].split("*");

			for(var j = 2; j < tableau2.length-1; j++) {
					html+="<tr id=\"ol_tr_"+count+"\">";
							html+="<td id=\"ol_check_"+count+"\">";
								html+="<div class=\"custom-control custom-checkbox\">";
									html+="<input type=\"checkbox\" class=\"custom-control-input\" id=\"dl_check_"+count+"\" >";
									html+="<label class=\"custom-control-label\" for=\"dl_check_"+count+"\"></label>";
								html+="</div>";										
							html+="</td>";

							html+="<td id=\"ol_dn_"+count+"\" >";
									html+=""+tableau2[0]+"</td>\n";  

							html+="<td id=\"ol_ip_"+count+"\" >";
									html+=""+tableau2[1]+"</td>\n";  

							tableau3 = tableau2[j].split(";");
							html+="<td id=\"ol_grp_"+count+"\" >";
									html+=""+tableau3[1]+"</td>\n";  

							html+="<td id=\"ol_ol_"+count+"\" >";
									html+=""+tableau3[0]+"</td>\n";

							html+="<td id=\"ol_op_"+count+"\" >";
									html+=""+tableau3[3]+"</td>\n";

							html+="<td id=\"ol_os_"+count+"\" >";
									html+=""+tableau3[2]+"</td>\n";   

							html+="<td id=\"ol_oc_"+count+"\" >";
									html+=""+tableau3[7]+"</td>\n";  

							html+="<td style=\"width:250px\" >";
							html+="<button id=\"ol_select_"+count+"\"  class=\"btn btn-info-group\" onclick=\"table_request_outputSelect("+count+")\"/>on</button></td>";

							html+="<td  id=\"ol_on_"+count+"\" style=\"cursor:pointer\" onclick=\"table_request_onoff("+count+",'on')\">";
									html+="on</td>\n";  

							html+="<td  id=\"ol_off_"+count+"\" style=\"cursor:pointer\" onclick=\"table_request_onoff("+count+",'off')\">";
									html+="off</td>\n";  

							html+="<td  id=\"tset_grp_"+count+"\" onchange=\"table_request_colorPicker("+count+")\">";
							html+="  </td>";

							html+="<td style=\"width:100\" >";
							html+="<input width=\"130\" type=\"range\" id=\"ol_sli_hbri"+count+"\" min=\"1\" max=\"255\" onchange=\"table_request_hBri("+count+")\" /></td>";

							html+="<td id=\"ol_tw_"+count+"\" >";
									html+=""+tableau3[6]+"</td>\n";  
	// var input = document.createElement('INPUT')
	// var picker = new jscolor(input)
	// picker.fromHSV(360 / 100 * i, 100, 100)

	// document.getElementById("ol_grp_"+count).appendChild(input)

							// html+="<td >";
							// html+='<td id="cp" class="jscolor" value="ab2567">     </td>';  
							// new jscolor(document.getElementById("cp" )[0]);

					html+="</tr>\n";
					count++;
			}        
		}

		html+="</tbody>"
		html+="</table>"

		document.getElementById('points-table').innerHTML=html;

		count = 0;
		for(var i = 0; i < tableau.length-1; i++) {
			tableau2 = tableau[i].split("*");
			for(var j = 2; j < tableau2.length-1; j++) {
					var input = document.createElement("INPUT");
					input.setAttribute ("id","ol_cp_"+count);
					input.setAttribute ("class","color_picker");

					var picker = new jscolor(input);
					tableau3 = tableau2[j].split(";");
					// picker.fromRGB(tableau3[4], tableau3[5], tableau3[6]);
					var ch = document.getElementById("tset_grp_"+count).appendChild(input);
					count++;
			}        
		}
}


// CMB FUNCTION
function cmb_set_group(chaine) {
   var tableau;
   tableau = chaine.split(";");
   
   var current = document.getElementById("input_group").value;
   var options = "<option selected='"+current+"'>"+current+"</option>"
   if (current == "") {
   		current = tableau[0];
   		options = "<option selected='"+current+"'>"+current+"</option>"
   }
   for(var i = 0; i < tableau.length - 1; i++) {

      options += "<option value='"+tableau[i]+"'>"+tableau[i]+"</option>"
   }
   options += "<option value='all'>all</option>";

   document.getElementById('input_group').innerHTML = options;
}


// REGB FUNCTION
function componentToHex(c) {
	var hex = c.toString(16);
	return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
   return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function rgbToComponents(rgb) {
	var components = {};

	rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
	components.r = parseInt(rgb[1]);
	components.g = parseInt(rgb[2]);
	components.b = parseInt(rgb[3]);

	return components;
}

function update(picker) {
	document.getElementById('hex-str').innerHTML = picker.toHEXString();
	document.getElementById('rgb-str').innerHTML = picker.toRGBString();

	document.getElementById('rgb').innerHTML =
		Math.round(picker.rgb[0]) + ', ' +
		Math.round(picker.rgb[1]) + ', ' +
		Math.round(picker.rgb[2]);

	document.getElementById('hsv').innerHTML =
		Math.round(picker.hsv[0]) + '&deg;, ' +
		Math.round(picker.hsv[1]) + '%, ' +
		Math.round(picker.hsv[2]) + '%';
}


// INITIIALISE

socket_init();

$('#timepicker').timepicker({
	uiLibrary: 'bootstrap4'
});

$(document).ready(function() {
	$("input#colorpicker").change(function() {
		console.log("COLOR: "+ this.color);
		$('select').formSelect();
	});
})

// $(document).ready(function() {
// 	$("td input[type=checkbox]").on('change', function (e) {
// 		console.log('change');
// 		row = $(this).closest('tr');
// 		console.log(row);
// 		console.log($(this).is(':checked'));
// 		if ($(this).is(':checked')) {
// 				row.addClass('selected');
// 		} else {
// 				row.removeClass('selected');
// 		}
// 	});


// });

// jQuery(function($) {
//   $("td input[type=checkbox]").on('change', function (e) {
//     console.log('change');
//     row = $(this).closest('tr');
//     console.log(row);
//     console.log($(this).is(':checked'));
//     if ($(this).is(':checked')) {
//         row.addClass('selected');
//     } else {
//         row.removeClass('selected');
//     }
//   });
// });



