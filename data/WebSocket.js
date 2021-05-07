var startAddrSet = 0;
var universeSet = 0;
var channelMode = 10;
var ch1_rIntens = 0;
var ch2_gIntens = 0;
var ch3_bIntens = 0;
var ch4_uvIntens = 0;
var ch5_strobe;
var ch5_strobeSpeed = 0;
var ch5_stobeMode = 255;
var ch5_masterDimmer = 255;
var ch6_masterDimmer = 255;
var ch7_modeSelect = 225;
var ch8_farbmakro = 0;
var ch8_progSel = 0;
var ch8_value;
var ch9_speedSenseSel = 255;
var ch10_dimCurve = 200;

var initRcvEsp = 0;

var saveValues = false;

var doOnce = true;

const definedColors = {
  RED: 1,
  YELLOW: 2,
  GREEN: 4
};
Object.freeze(definedColors);

function sendVal() {  //tiggert im server die funktion um Werte vom Client (Browser) an den Server (ESP) zu schicken

   if (!myAjax){
      return false;
   } else {
	   switch (channelMode){
		   case 4:
						myAjax.open("GET",Url+"dataClient2Server?chN=" + channelMode + "&ch1=" + ch1_rIntens + "&ch2=" + ch2_gIntens + "&ch3=" + ch3_bIntens + "&ch4=" + ch4_uvIntens + "&ch5=-1&ch6=-1&ch7=-1&ch8=-1&ch9=-1&ch10=-1&save=" + saveValues + "&univ=" + universeSet + "&startAddr=" + startAddrSet + "&iRcv=" + initRcvEsp,true);
						break;
		   case 5:
						myAjax.open("GET",Url+"dataClient2Server?chN=" + channelMode + "&ch1=" + ch1_rIntens + "&ch2=" + ch2_gIntens + "&ch3=" + ch3_bIntens + "&ch4=" + ch4_uvIntens + "&ch5=" + ch5_masterDimmer + "&ch6=-1&ch7=-1&ch8=-1&ch9=-1&ch10=-1&save=" + saveValues + "&univ=" + universeSet + "&startAddr=" + startAddrSet + "&iRcv=" + initRcvEsp,true);
						break;
		   case 6:
						if((ch5_stobeMode == 64) || (ch5_stobeMode == 128) || (ch5_stobeMode == 192)){
							ch5_strobe = ch5_strobeSpeed+ch5_stobeMode;
						}else{
							ch5_strobe = ch5_stobeMode;
						}
						myAjax.open("GET",Url+"dataClient2Server?chN=" + channelMode + "&ch1=" + ch1_rIntens + "&ch2=" + ch2_gIntens + "&ch3=" + ch3_bIntens + "&ch4=" + ch4_uvIntens + "&ch5=" + ch5_strobe + "&ch6=" + ch6_masterDimmer + "&ch7=-1&ch8=-1&ch9=-1&ch10=-1&save=" + saveValues + "&univ=" + universeSet + "&startAddr=" + startAddrSet + "&iRcv=" + initRcvEsp,true);
						break;
		   case 9:
						if((ch5_stobeMode == 64) || (ch5_stobeMode == 128) || (ch5_stobeMode == 192)){
							ch5_strobe = ch5_strobeSpeed+ch5_stobeMode;
						}else{
							ch5_strobe = ch5_stobeMode;
						}
						ch8_value = ch8_progSel*16;
						if((ch7_modeSelect == 25) || (ch7_modeSelect == 75)){
							ch8_value = ch8_farbmakro;
						}
						myAjax.open("GET",Url+"dataClient2Server?chN=" + channelMode + "&ch1=" + ch1_rIntens + "&ch2=" + ch2_gIntens + "&ch3=" + ch3_bIntens + "&ch4=" + ch4_uvIntens + "&ch5=" + ch5_strobe + "&ch6=" + ch6_masterDimmer + "&ch7=" + ch7_modeSelect + "&ch8=" + ch8_value + "&ch9=" + ch9_speedSenseSel + "&ch10=-1&save=" + saveValues + "&univ=" + universeSet + "&startAddr=" + startAddrSet + "&iRcv=" + initRcvEsp,true);
						break;
			case 10:
						if((ch5_stobeMode == 64) || (ch5_stobeMode == 128) || (ch5_stobeMode == 192)){
							ch5_strobe = ch5_strobeSpeed+ch5_stobeMode;
						}else{
							ch5_strobe = ch5_stobeMode;
						}
						ch8_value = ch8_progSel*16;
						if((ch7_modeSelect == 25) || (ch7_modeSelect == 75)){
							ch8_value = ch8_farbmakro;
						}
						myAjax.open("GET",Url+"dataClient2Server?chN=" + channelMode + "&ch1=" + ch1_rIntens + "&ch2=" + ch2_gIntens + "&ch3=" + ch3_bIntens + "&ch4=" + ch4_uvIntens + "&ch5=" + ch5_strobe + "&ch6=" + ch6_masterDimmer + "&ch7=" + ch7_modeSelect + "&ch8=" + ch8_value + "&ch9=" + ch9_speedSenseSel + "&ch10=" + ch10_dimCurve + "&save=" + saveValues + "&univ=" + universeSet + "&startAddr=" + startAddrSet + "&iRcv=" + initRcvEsp,true);
						break;
		   default:
						break;
	   }
      myAjax.send();
      saveValues = false;
   }
}

function fhemConfig(){
   // document.getElementById('Online').style.background = "red";
   if (!myAjax){
      return false;
   } else {
      myAjax.open("GET",Url+"fhemConfig.html",true);
      myAjax.send();
   }
}

function channel1to4deactivate(){
	//deactivate ch1 controls
	document.getElementById('ch1Slider').className = 'disabled';
	document.getElementById('ch1Slider').disabled = true;
	document.getElementById('ch1Set').disabled = true;
	document.getElementById('ch1Submit').disabled = true;
	//deactivate ch2 controls
	document.getElementById('ch2Slider').className = 'disabled';
	document.getElementById('ch2Slider').disabled = true;
	document.getElementById('ch2Set').disabled = true;
	document.getElementById('ch2Submit').disabled = true;
	//deactivate ch3 controls
	document.getElementById('ch3Slider').className = 'disabled';
	document.getElementById('ch3Slider').disabled = true;
	document.getElementById('ch3Set').disabled = true;
	document.getElementById('ch3Submit').disabled = true;
	//deactivate ch4 controls
	document.getElementById('ch4Slider').className = 'disabled';
	document.getElementById('ch4Slider').disabled = true;
	document.getElementById('ch4Set').disabled = true;
	document.getElementById('ch4Submit').disabled = true;
}

function channel1to4activate(){
	//deactivate ch1 controls
	document.getElementById('ch1Slider').className = 'enabled';
	document.getElementById('ch1Slider').disabled = false;
	document.getElementById('ch1Set').disabled = false;
	document.getElementById('ch1Submit').disabled = false;
	//deactivate ch2 controls
	document.getElementById('ch2Slider').className = 'enabled';
	document.getElementById('ch2Slider').disabled = false;
	document.getElementById('ch2Set').disabled = false;
	document.getElementById('ch2Submit').disabled = false;
	//deactivate ch3 controls
	document.getElementById('ch3Slider').className = 'enabled';
	document.getElementById('ch3Slider').disabled = false;
	document.getElementById('ch3Set').disabled = false;
	document.getElementById('ch3Submit').disabled = false;
	//deactivate ch4 controls
	document.getElementById('ch4Slider').className = 'enabled';
	document.getElementById('ch4Slider').disabled = false;
	document.getElementById('ch4Set').disabled = false;
	document.getElementById('ch4Submit').disabled = false;
}

function setStrobe(stobeModeSel){
	ch5_stobeMode = parseInt(stobeModeSel);
	console.log('SetStobe: ' + ch5_stobeMode);

	switch (ch5_stobeMode){
    case 0:
    case 255:
		//Steuerelement aktivieren/deaktivieren
		document.getElementById('strobeSpeedSet').disabled = true;
		sendVal(); // trigger übertragung Client->Server (Browser->ESP)
		break;
    case 64:
    case 128:
    case 192:
		//Steuerelement aktivieren/deaktivieren
		document.getElementById('strobeSpeedSet').disabled = false;
		sendVal(); // trigger übertragung Client->Server (Browser->ESP)
		break;
    default:
		document.getElementById('strobeSpeedSet').disabled = true;
		console.log('Unknown strobe mode - Selecting 255 (LED Ein)');
		channelMode = 255;
		sendVal(); // trigger übertragung Client->Server (Browser->ESP)
	}
}

function setDimmerkurve(dimCurveSel){
	ch10_dimCurve = parseInt(dimCurveSel);
	console.log('DimCurve: ' + ch10_dimCurve);
	sendVal(); // trigger übertragung Client->Server (Browser->ESP)
}

function handleModeChange(){
	if((ch7_modeSelect==25) || (ch7_modeSelect==75)){ //Dimmer-Modus || Farbton-Modus --> Deactivate ch1-4 and 8+9 controls (but still display them in "farbmakro-style")
		document.getElementById('ch8_farbmakro_tableRow').style.display = 'table-row';
		document.getElementById('ch8_progSel_tableRow').style.display = 'none';
		if(ch7_modeSelect==25){
			channel1to4activate();
			//deactivate ch8 controls
			document.getElementById('ch8slider_farbmakro').className = 'disabled';
			document.getElementById('ch8slider_farbmakro').disabled = true;
			document.getElementById('farbtonSetInput').disabled = true;
			document.getElementById('farbtonSetSubmit').disabled = true;
		} else {
			channel1to4deactivate();
			//activate ch8 controls
			document.getElementById('ch8slider_farbmakro').className = 'enabled';
			document.getElementById('ch8slider_farbmakro').disabled = false;
			document.getElementById('farbtonSetInput').disabled = false;
			document.getElementById('farbtonSetSubmit').disabled = false;
		}
		//deactivate ch9 controls
		document.getElementById('ch9slider').className = 'disabled';
		document.getElementById('ch9slider').disabled = true;
		document.getElementById('speedSenseSetInput').disabled = true;
		document.getElementById('speedSenseSetSubmit').disabled = true;
	} else {
 		channel1to4deactivate();
		document.getElementById('ch8_farbmakro_tableRow').style.display = 'none';
		document.getElementById('ch8_progSel_tableRow').style.display = 'table-row';
		//activate ch8 controls
		document.getElementById('ch8slider_farbmakro').className = 'enabled';
		document.getElementById('ch8slider_farbmakro').disabled = false;
		document.getElementById('farbtonSetInput').disabled = false;
		document.getElementById('farbtonSetSubmit').disabled = false;
		//activate ch9 controls
		document.getElementById('ch9slider').className = 'enabled';
		document.getElementById('ch9slider').disabled = false;
		document.getElementById('speedSenseSetInput').disabled = false;
		document.getElementById('speedSenseSetSubmit').disabled = false;
	 }
}	

function limitValue(valueToCheck, lowerLimit, upperLimit, InputFieldId){
	if (valueToCheck > upperLimit ) { 
		valueToCheck = upperLimit;
		document.getElementById(InputFieldId).value = valueToCheck;
	}
	if (valueToCheck < lowerLimit ) { 
		valueToCheck = lowerLimit;
		document.getElementById(InputFieldId).value = valueToCheck;
	}
	return valueToCheck;
}

function submitVal(InputFieldId, SliderId){
	//read value from edit field
	//update slider-Wert
	//sendVal
	console.log('Enter data: ' + InputFieldId + ' - Slider Id: ' + SliderId);

	switch (InputFieldId){
		case 'addrInput':
			//read Input-field
			startAddrSet = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			startAddrSet = limitValue(startAddrSet, 0, 511, InputFieldId);
			document.getElementById(SliderId).value = startAddrSet;
			console.log('Start-Address selected: ' + startAddrSet);
			document.getElementById('ch1td').innerHTML=(parseInt(startAddrSet))%512;
			document.getElementById('ch2td').innerHTML=(parseInt(startAddrSet)+1)%512;
			document.getElementById('ch3td').innerHTML=(parseInt(startAddrSet)+2)%512;
			document.getElementById('ch4td').innerHTML=(parseInt(startAddrSet)+3)%512;
			document.getElementById('ch5_1td').innerHTML=(parseInt(startAddrSet)+4)%512;
			document.getElementById('ch5_2td').innerHTML=(parseInt(startAddrSet)+4)%512;
			document.getElementById('ch6td').innerHTML=(parseInt(startAddrSet)+5)%512;
			document.getElementById('ch7td').innerHTML=(parseInt(startAddrSet)+6)%512;
			document.getElementById('ch8_1td').innerHTML=(parseInt(startAddrSet)+7)%512;
			document.getElementById('ch8_2td').innerHTML=(parseInt(startAddrSet)+7)%512;			
			document.getElementById('ch9td').innerHTML=(parseInt(startAddrSet)+8)%512;			
			document.getElementById('ch10td').innerHTML=(parseInt(startAddrSet)+9)%512;			
	
			break;
		case 'universeInput':
			//read Input-field
			universeSet = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			universeSet = limitValue(universeSet, 0, 511, InputFieldId);
			document.getElementById(SliderId).value = universeSet;
			console.log('Universe selected: ' + universeSet);
			break;
		case 'speedSenseSetInput':
			//read Input-field
			ch9_speedSenseSel = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch9_speedSenseSel = limitValue(ch9_speedSenseSel, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch9_speedSenseSel;
			console.log('Speed/Sensitivity value: ' + ch9_speedSenseSel);
			break;
		case 'progSelSetInput':
			//read Input-field
			ch8_progSel = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch8_progSel = limitValue(ch8_progSel, 0, 15, InputFieldId);
			document.getElementById(SliderId).value = ch8_progSel;
			console.log('Program selected: ' + ch8_progSel);
			break;
		case 'farbtonSetInput':
			//read Input-field
			ch8_farbmakro = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch8_farbmakro = limitValue(ch8_farbmakro, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch8_farbmakro;
			console.log('Farbmakro selected: ' + ch8_farbmakro);
			break;
		case 'ch6Edit':
			//read Input-field
			ch6_masterDimmer = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch6_masterDimmer = limitValue(ch6_masterDimmer, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch6_masterDimmer;
			console.log('MasterDimmer value: ' + ch6_masterDimmer);
			break;
		case 'ch5_dimmer_valEdit':
			//read Input-field
			ch5_masterDimmer = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch5_masterDimmer = limitValue(ch5_masterDimmer, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch5_masterDimmer;
			console.log('MasterDimmer value: ' + ch5_masterDimmer);
			break;
		case 'strobeSpeedSet':
			//read Input-field
			ch5_strobeSpeed = parseInt(document.getElementById(InputFieldId).value);
			//NO slider available to be updated...
			console.log('Strobe-speed value: ' + ch5_strobeSpeed);
			break;
		case 'ch4Set':
			//read Input-field
			ch4_uvIntens = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch4_uvIntens = limitValue(ch4_uvIntens, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch4_uvIntens;
			console.log('UV value: ' + ch4_uvIntens);
			break;
		case 'ch3Set':
			//read Input-field
			ch3_bIntens = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch3_bIntens = limitValue(ch3_bIntens, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch3_bIntens;
			console.log('Blue value: ' + ch3_bIntens);
			break;
		case 'ch2Set':
			//read Input-field
			ch2_gIntens = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch2_gIntens = limitValue(ch2_gIntens, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch2_gIntens;
			console.log('Green value: ' + ch2_gIntens);
			break;
		case 'ch1Set':
			//read Input-field
			ch1_rIntens = document.getElementById(InputFieldId).value
			//update slider (may not be necessary - but also does not harm (if one only used the input filed - and not the slider)
			ch1_rIntens = limitValue(ch1_rIntens, 0, 255, InputFieldId);
			document.getElementById(SliderId).value = ch1_rIntens;
			console.log('Red value: ' + ch1_rIntens);
			break;
		default:
			break;
	}
	sendVal(); // trigger übertragung Client->Server (Browser->ESP)
}

function setSliderVal(elementValue, elementId){
	document.getElementById(elementId).value = elementValue;
}

function setChannelMode(numChMode){ //Wird über HTML-Seite aufgerufen und schickt Werte vom Client (Browser)-> Server (ESP)
	channelMode = parseInt(numChMode);
	console.log('channelMode: ' + channelMode);

	switch (channelMode){
      case 4:
        //Steuerelement aktivieren/deaktivieren
 		channel1to4activate();
        document.getElementById('ch1tableRow').style.display = 'table-row';
        document.getElementById('ch2tableRow').style.display = 'table-row';
        document.getElementById('ch3tableRow').style.display = 'table-row';
        document.getElementById('ch4tableRow').style.display = 'table-row';
        document.getElementById('ch5_strobe_tableRow').style.display = 'none';
        document.getElementById('ch5_dimmer_tableRow').style.display = 'none';
        document.getElementById('ch6tableRow').style.display = 'none';
        document.getElementById('ch7tableRow').style.display = 'none';
        document.getElementById('ch8_farbmakro_tableRow').style.display = 'none';
        document.getElementById('ch8_progSel_tableRow').style.display = 'none';
        document.getElementById('ch9tableRow').style.display = 'none';
        document.getElementById('ch10tableRow').style.display = 'none';
        break;
      case 5:
        //Steuerelement aktivieren/deaktivieren
 		channel1to4activate();
        document.getElementById('ch1tableRow').style.display = 'table-row';
        document.getElementById('ch2tableRow').style.display = 'table-row';
        document.getElementById('ch3tableRow').style.display = 'table-row';
        document.getElementById('ch4tableRow').style.display = 'table-row';
        document.getElementById('ch5_strobe_tableRow').style.display = 'none';
        document.getElementById('ch5_dimmer_tableRow').style.display = 'table-row';
        document.getElementById('ch6tableRow').style.display = 'none';
        document.getElementById('ch7tableRow').style.display = 'none';
        document.getElementById('ch8_farbmakro_tableRow').style.display = 'none';
        document.getElementById('ch8_progSel_tableRow').style.display = 'none';
        document.getElementById('ch9tableRow').style.display = 'none';
        document.getElementById('ch10tableRow').style.display = 'none';
        break;
    case 6:
        //Steuerelement deaktivieren
 		channel1to4activate();
        document.getElementById('ch1tableRow').style.display = 'table-row';
        document.getElementById('ch2tableRow').style.display = 'table-row';
        document.getElementById('ch3tableRow').style.display = 'table-row';
        document.getElementById('ch4tableRow').style.display = 'table-row';
        document.getElementById('ch5_strobe_tableRow').style.display = 'table-row';
        document.getElementById('ch5_dimmer_tableRow').style.display = 'none';
        document.getElementById('ch6tableRow').style.display = 'table-row';
        document.getElementById('ch7tableRow').style.display = 'none';
        document.getElementById('ch8_farbmakro_tableRow').style.display = 'none';
        document.getElementById('ch8_progSel_tableRow').style.display = 'none';
        document.getElementById('ch9tableRow').style.display = 'none';
        document.getElementById('ch10tableRow').style.display = 'none';
        break;
    case 9:
        //Steuerelement aktivieren/deaktivieren
        document.getElementById('ch1tableRow').style.display = 'table-row';
        document.getElementById('ch2tableRow').style.display = 'table-row';
        document.getElementById('ch3tableRow').style.display = 'table-row';
        document.getElementById('ch4tableRow').style.display = 'table-row';
        document.getElementById('ch5_strobe_tableRow').style.display = 'table-row';
        document.getElementById('ch5_dimmer_tableRow').style.display = 'none';
        document.getElementById('ch6tableRow').style.display = 'table-row';
        document.getElementById('ch7tableRow').style.display = 'table-row';
		handleModeChange();
        document.getElementById('ch9tableRow').style.display = 'table-row';
        document.getElementById('ch10tableRow').style.display = 'none';
        break;
      case 10:
        //Steuerelement aktivieren/deaktivieren
        document.getElementById('ch1tableRow').style.display = 'table-row';
        document.getElementById('ch2tableRow').style.display = 'table-row';
        document.getElementById('ch3tableRow').style.display = 'table-row';
        document.getElementById('ch4tableRow').style.display = 'table-row';
        document.getElementById('ch5_strobe_tableRow').style.display = 'table-row';
        document.getElementById('ch5_dimmer_tableRow').style.display = 'none';
        document.getElementById('ch6tableRow').style.display = 'table-row';
        document.getElementById('ch7tableRow').style.display = 'table-row';
		handleModeChange();
        document.getElementById('ch9tableRow').style.display = 'table-row';
        document.getElementById('ch10tableRow').style.display = 'table-row';
        break;
      default:
        console.log('Unknown mode - Selecting 10-channel-mode...');
        channelMode = 10;
   }
   sendVal(); // trigger übertragung Client->Server (Browser->ESP)
}

function setMode(numMode){ //Wird über HTML-Seite aufgerufen und schickt Werte vom Client (Browser)-> Server (ESP)

	ch7_modeSelect = parseInt(numMode);
	console.log('Mode: ' + ch7_modeSelect);
	// allowed values: 25, 75, 125, 175, 225;
	handleModeChange();
	switch (ch7_modeSelect){
		// changing the mode has immediate effect only if value==25 or ==225;
		case 25:
		case 225:
			sendVal(); // trigger übertragung Client->Server (Browser->ESP)
			break;
		// For modes 75, 125, 175 - an additonal 5 bit value needs to be set 1st
		default:
			sendVal(); // trigger übertragung Client->Server (Browser->ESP)
			break;
	}
}

function saveAll(){ //Wird über HTML-Seite aufgerufen und schickt Werte vom Client (Browser)-> Server (ESP)
   saveValues = true;
   sendVal(); // trigger übertragung Client->Server (Browser->ESP)
}

function loadDefaultsWeb(){ //Wird über HTML-Seite aufgerufen und schickt Werte vom Client (Browser)-> Server (ESP)
	startAddrSet = 0;
	universeSet = 0;
	channelMode = 10;
	ch1_rIntens = 0;
	ch2_gIntens = 0;
	ch3_bIntens = 0;
	ch4_uvIntens = 0;
	ch5_strobe;
	ch5_strobeSpeed = 0;
	ch5_stobeMode = 255;
	ch5_masterDimmer = 255;
	ch6_masterDimmer = 255;
	ch7_modeSelect = 225;
	ch8_farbmakro = 0;
	ch8_progSel = 0;
	ch8_value;
	ch9_speedSenseSel = 255;
	ch10_dimCurve = 200;
	
	loadDefaults();
}

function loadDefaults(){ //Wird über HTML-Seite aufgerufen und schickt Werte vom Client (Browser)-> Server (ESP)
	
	console.log('loadDefaults...');

	document.getElementById('addrInput').value = startAddrSet;
	document.getElementById('addrSelSlider').value = startAddrSet;

	document.getElementById('universeInput').value = universeSet;
	document.getElementById('univSelSlider').value = universeSet;

	document.getElementById('dimCurveSet').value = ch10_dimCurve;
	
	document.getElementById('speedSenseSetInput').value = ch9_speedSenseSel;
	document.getElementById('ch9slider').value = ch9_speedSenseSel;
	
	document.getElementById('progSelSetInput').value = ch8_progSel;
	document.getElementById('ch8slider_progSel').value = ch8_progSel;

	document.getElementById('farbtonSetInput').value = ch8_farbmakro;
	document.getElementById('ch8slider_farbmakro').value = ch8_farbmakro;

	document.getElementById('progModeSet').value = ch7_modeSelect;
	handleModeChange();
	
	document.getElementById('ch6Edit').value = ch6_masterDimmer;
	document.getElementById('ch6Slider').value = ch6_masterDimmer;
	
	document.getElementById('ch5_dimmer_valEdit').value = ch5_masterDimmer;
	document.getElementById('ch5Slider').value = ch5_masterDimmer;
	
	document.getElementById('strobeSpeedSet').value = ch5_strobeSpeed;
	document.getElementById('strobeModeSet').value = ch5_stobeMode;
	
	document.getElementById('ch4Set').value = ch4_uvIntens;
	document.getElementById('ch4Slider').value = ch4_uvIntens;

	document.getElementById('ch3Set').value = ch3_bIntens;
	document.getElementById('ch3Slider').value = ch3_bIntens;

	document.getElementById('ch2Set').value = ch2_gIntens;
	document.getElementById('ch2Slider').value = ch2_gIntens;

	document.getElementById('ch1Set').value = ch1_rIntens;
	document.getElementById('ch2Slider').value = ch1_rIntens;

	setChannelMode(channelMode);

	setStrobe(ch5_stobeMode); //setStrobe ALWAYS uses SendVal already...
	//sendVal(); // trigger übertragung Client->Server (Browser->ESP)
}

function start (){
Url="/";
if (window.XMLHttpRequest) { // Mozilla, Safari,...
      myAjax = new XMLHttpRequest();
      if (myAjax.overrideMimeType) {
        myAjax.overrideMimeType('text/xml');
      }
    } else if (window.ActiveXObject) { // IE
      try {
        myAjax = new ActiveXObject("Msxml2.XMLHTTP");
      } catch (e) {
        try {
          myAjax = new ActiveXObject("Microsoft.XMLHTTP");
        } catch (e) {}
      }
    }
  myAjax.onreadystatechange=LesenAjax;
  /*
    document.getElementById('univSelSlider').value=0;  //Setzen des default slider-Werts
    document.getElementById('universeInput').value=0;  //Setzen des default Input-Werts
  */
  setInterval(aktualisieren, 500); // Funktion "aktualisieren" wird alle 500 ms aufgerufen, um Werte vom Server zum Client zu übertagen - der dann die Werte entgegen nimmt
}

function aktualisieren(){ //Frägt beim ESP (Server) alle 500ms die Werte neu an
   // document.getElementById('Online').style.background = "red";
   if (!myAjax){
      return false;
   } else {
      myAjax.open("GET",Url+"triggerServer2Client",true);
      myAjax.send();
   }
}

function LesenAjax()
{
	var chX = [];
	var numChEsp;
	var startAddrEsp;
	var univEsp;
	var outpString="";
	
    if (myAjax.readyState==4 && myAjax.status==200)
    {
		var stringRcv;
		stringRcv = myAjax.responseText;

		var res = stringRcv.split("|");
		var i=0;

		for(i = 0; i < channelMode; i++) {
			chX[i] = res[i];
			if(i==channelMode-1){
				outpString = outpString + chX[i];
			}
			else{
				outpString = outpString + chX[i] + ",";
			}
		}
		numChEsp = res[i];
		startAddrEsp = res[i+1];
		univEsp = res[i+2];
		initRcvEsp = parseInt(res[i+3]);
		
		console.log('ch[0...9]: ' + outpString);
		console.log('#ChEsp/startAddr/univEsp/initialReceive: ' + numChEsp + "/" + startAddrEsp +"/" + univEsp +"/" + initRcvEsp);

		if(doOnce){
			console.log('Load default once from ESP!');
			doOnce = false;

			startAddrSet = startAddrEsp;
			universeSet = univEsp;
			channelMode = numChEsp;
			ch1_rIntens = chX[0];
			ch2_gIntens = chX[1];
			ch3_bIntens = chX[2];
			ch4_uvIntens = chX[3];
			ch5_strobeSpeed = chX[4]&63;
			ch5_stobeMode = chX[4]&0xC0;;
			
			console.log('ch5_strobeSpeed (0...63): ' + ch5_strobeSpeed);
			console.log('ch5_stobeMode (64, 128, 192): ' + ch5_stobeMode);
			
			ch5_masterDimmer = chX[4];
			ch6_masterDimmer = chX[5];
			ch7_modeSelect = chX[6];
			ch8_farbmakro = chX[7];
			ch8_progSel = (chX[7]>>4)&0x0F;
			ch9_speedSenseSel = chX[8];
			ch10_dimCurve = chX[9];

			loadDefaults();
		}

		document.getElementById('numChanCur').innerHTML=numChEsp;
		document.getElementById('univCur').innerHTML=univEsp;
		document.getElementById('addrCur').innerHTML=startAddrEsp;

		document.getElementById('ch1cur').innerHTML=chX[0];
		document.getElementById('ch2cur').innerHTML=chX[1];
		document.getElementById('ch4cur').innerHTML=chX[2];
		document.getElementById('ch4cur').innerHTML=chX[3];
		document.getElementById('ch5_1cur').innerHTML=chX[4];
		document.getElementById('ch5_2cur').innerHTML=chX[4];
		document.getElementById('ch6cur').innerHTML=chX[5];
		document.getElementById('ch7cur').innerHTML=chX[6];
		document.getElementById('ch8_1cur').innerHTML=chX[7];
		document.getElementById('ch8_2cur').innerHTML=chX[7];
		document.getElementById('ch9cur').innerHTML=chX[8];
		document.getElementById('ch10cur').innerHTML=chX[9];

   }
}


