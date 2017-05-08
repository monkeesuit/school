function Edit(name) {
  $.ajax({
         url: '/GetPhotoByName',
         data: {name: $(name).text()},
         type: 'POST',
         success: function(response) {
         console.log(response);
         var data = JSON.parse(response);
         var target = '#element-' + $(name).attr('id');
         console.log(target);
         $('#addform').tmpl(data).insertAfter(target);
         },
         error: function(error) {
         console.log(error);
         }
         });
  }
