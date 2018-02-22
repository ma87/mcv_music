
var timeSlide = 5000;

var reverb_enabled = 1;

$(document).ready(function()
{
	var start_volume = 1.0 / 5.656854249492;
	var audio_data = {"request": "input_volume", "input_volume": start_volume};

  var inputVolumeBar = document.querySelector('#InputVolume');
	inputVolumeBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, inputVolumeBar, updateAudioData, audio_data, "input_volume");
	}, false);


  reverb_data = {"request": "reverb", "reverb_enabled": reverb_enabled, "reverb_type": 0, "reverb_volume": start_volume, "room_size": 0.1, "damp": 0.1, "dry": 0.3, "wet": 0.3, "width": 1.0};

  $("#ReverbEnabled").click(function(e)
  {
    reverb_data["request"] = "reverb_enabled";
    reverb_data["reverb_enabled"] = 1 - reverb_data["reverb_enabled"];
    if (reverb_data["reverb_enabled"]==1)
    {
      $(this).text("ON");
    }
    else
    {
      $(this).text("OFF");
    }
    updateReverb(reverb_data);
  });

  $("#ReverbTypeSELF").click(function(e)
	{
    reverb_data["request"] = "reverb_type";
    reverb_data["reverb_type"] = 0;
    updateReverb(reverb_data);
  });

  $("#ReverbTypePROG").click(function(e)
	{
    reverb_data["request"] = "reverb_type";
    reverb_data["reverb_type"] = 30;
    updateReverb(reverb_data);
  });

  $("#ReverbTypePROG2").click(function(e)
	{
    reverb_data["request"] = "reverb_type";
    reverb_data["reverb_type"] = 31;
    updateReverb(reverb_data);
  });

  $("#ReverbTypeZREV").click(function(e)
	{
    reverb_data["request"] = "reverb_type";
    reverb_data["reverb_type"] = 40;
    updateReverb(reverb_data);
  });

  $("#ReverbTypeZREV2").click(function(e)
	{
    reverb_data["request"] = "reverb_type";
    reverb_data["reverb_type"] = 41;
    updateReverb(reverb_data);
  });

  var reverbVolumeSizeBar = document.querySelector('#ReverbVolume');
	reverbVolumeSizeBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, reverbVolumeSizeBar, updateReverb, reverb_data, "reverb_volume");
	}, false);

	var roomSizeBar = document.querySelector('#RoomSize');
	roomSizeBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, roomSizeBar, updateReverb, reverb_data, "room_size");
	}, false);

	var dampBar = document.querySelector('#Damp');
	dampBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, dampBar, updateReverb, reverb_data, "damp");
	}, false);

	var dryBar = document.querySelector('#Dry');
	dryBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, dryBar, updateReverb, reverb_data, "dry");
	}, false);

	var wetBar = document.querySelector('#Wet');
	wetBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, wetBar, updateReverb, reverb_data, "wet");
	}, false);

  var widthBar = document.querySelector('#Width');
	widthBar.addEventListener("click", function(e)
	{
		setPositionProgressBar(e, widthBar, updateReverb, reverb_data, "width");
	}, false);
});

function setPositionProgressBar(evt, progressBar, callback, data, parameter)
{
	progressBar.value = (evt.clientX - progressBar.getBoundingClientRect().left)/(progressBar.getBoundingClientRect().right - progressBar.getBoundingClientRect().left);

  data["request"] = parameter;
	data[parameter] = progressBar.value;
	callback(data);
}

function updateReverb(reverb_data)
{
    $.post('/setReverb', reverb_data, function(data) {

    });
}

function updateAudioData(audio_data)
{
  $.post('/setAudio', audio_data, function(data) {

  });
}
