$(document).ready(function(){
	$('.node-type-framework-term #edit-delete').click(function(){
		var answer = confirm("You are about to delete a taxonomy (PDGuide item) All of it's children will also be deleted! Are you sure you want to do this?");
		if(!answer){
			return false;
		}
	});
});