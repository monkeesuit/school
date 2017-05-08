$(function() {
  $('#file').fileupload({
                        url: 'Upload',
                        dataType: 'json',
                        add: function(e, data) {
                        data.submit();
                        },
                        success: function(response, status) {
                        console.log(response);
                        $('#preview').attr('src','static/Images/'+response.filename);
                        $('#filepath').val('static/Images/'+response.filename);
                        },
                        error: function(error) {
                        console.log(error);
                        }
                        });
  })
