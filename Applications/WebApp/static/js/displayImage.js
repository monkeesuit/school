function DisplayImage(name,filepath,caption) {
  var div = $('<div>');
  var img = $('<img>').attr({
                            'src': filepath,
                            'style': 'height: 150px; width: 150px; display: block'
                            });
  var caption = $('<p>').text(caption);
  
  div.append(img);
  div.append(caption);
  
  return div;
}
