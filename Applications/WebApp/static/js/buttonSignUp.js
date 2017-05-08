$(function() {
  $('#buttonSignUp').click(function() {
                           
                           $.ajax({
                                  url: '/SignUp',
                                  data: $('form').serialize(),
                                  type: 'POST',
                                  success: function(response) {
                                  },
                                  error: function(error) {
                                  }
                                  });
                           });
  });
