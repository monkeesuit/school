function GetEditPhotos(page) {
  
  var offset = (page - 1) * 10;
  
  $.ajax({
         url: '/GetUserPhotos',
         type: 'POST',
         data : {o: offset},
         success: function(response) {
         
         var itemsperpage = 10;
         
         var data = JSON.parse(response);
         var info = data[0];
         
         
         $('#uploads').empty();
         $('#additem').tmpl(info).appendTo('#uploads');
         
         
         
         var total = data[1]['total'];
         var pagecount = total/itemsperpage;
         var extrapages = total%itemsperpage;
         
         if(extrapages !=0 ){
         pagecount = Math.floor(pagecount)+1;
         }
         
         
         $('#pagination').empty();
         
         var pagestart = $('#start').val();
         var pageend = $('#end').val();
         
         if(pagestart>5){
         var previousbutton = $('<a/>').attr({'href':'#'}).append($('<span/>').html('&laquo;'));
         $(previousbutton).click(function(){
                                 $('#start').val(Number(pagestart) - 5);
                                 $('#end').val(Number(pagestart) - 5 + 4);
                                 GetEditPhotos(Number(pagestart) - 5);
                                 });
         }
         var previous = $('<li/>').append(previousbutton);
         $('#pagination').append(previous);
         
         for (var i=Number(pagestart);i<=Number(pageend);i++) {
         if (i > pagecount) {
         break;
         }
         
         var pagebutton = $('<a/>').attr('href', '#').text(i);
         $(pagebutton).click(function(i) {
                             return function() {
                             GetEditPhotos(i);
                             }
                             }(i));
         var pagenumbers = $('<li/>').append(pagebutton);
         $('#pagination').append(pagenumbers);
         }
         
         if((Number(pagestart) + 5) <= pagecount){
         var nextbutton = $('<a/>').attr({'href':'#'}).append($('<span/>').html('&raquo;'));
         $(previousbutton).click(function(){
                                 $('#start').val(Number(pagestart) + 5);
                                 $('#end').val(Number(pagestart) + 5 + 4);
                                 GetEditPhotos(Number(pagestart) + 5);
                                 });
         }
         var next = $('<li/>').append(nextbutton);
         $('#pagination').append(next);
         },
         error: function(error) {
         console.log(error);
         }
         });
  }
