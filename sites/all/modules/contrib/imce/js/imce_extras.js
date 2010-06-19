// $Id: imce_extras.js,v 1.3.2.1 2010/05/15 13:32:44 ufku Exp $
//This pack implemets: keyboard shortcuts, file sorting, resize bars, and inline thumbnail preview.

(function($) {

//add onload hook. unshift to make sure it runs first after imce loads.
imce.hooks.load.unshift(function () {
  imce.NW = imce.el('navigation-wrapper');
  imce.BW = imce.el('browse-wrapper');
  imce.PW = imce.el('preview-wrapper');
  //add scale calculator for resizing.
  $('#edit-width, #edit-height').focus(function () {
    var fid, r, w, isW, val;
    if (fid = imce.vars.prvfid) {
      isW = this.id == 'edit-width', val =  imce.el(isW ? 'edit-height' : 'edit-width').value*1;
      if (val && (w = imce.isImage(fid)) && (r = imce.fids[fid].cells[3].innerHTML*1 / w))
        this.value = Math.round(isW ? val/r : val*r);
    }
  });
});

/**************** SHORTCUTS ********************/
var F = null;
imce.initiateShortcuts = function () {
  $(imce.NW).attr('tabindex', '0').keydown(function (e) {
    if (F = imce.dirKeys['k'+ e.keyCode]) return F(e);
  });
  $(imce.FLW).attr('tabindex', '0').keydown(function (e) {
    if (F = imce.fileKeys['k'+ e.keyCode]) return F(e);
  }).focus();
};

//shortcut key-function pairs for directories
imce.dirKeys = {
  k35: function (e) {//end-home. select first or last dir
    var L = imce.tree['.'].li;
    if (e.keyCode == 35) while (imce.hasC(L, 'expanded')) L = L.lastChild.lastChild;
    $(L.childNodes[1]).click().focus();
  },
  k37: function (e) {//left-right. collapse-expand directories.(right may also move focus on files)
    var L, B = imce.tree[imce.conf.dir], right = e.keyCode == 39;
    if (B.ul && (right ^ imce.hasC(L = B.li, 'expanded')) ) $(L.firstChild).click();
    else if (right) $(imce.FLW).focus();
  },
  k38: function (e) {//up. select the previous directory
    var B = imce.tree[imce.conf.dir];
    if (L = B.li.previousSibling) {
      while (imce.hasC(L, 'expanded')) L = L.lastChild.lastChild;
      $(L.childNodes[1]).click().focus();
    }
    else if ((L = B.li.parentNode.parentNode) && L.tagName == 'LI') $(L.childNodes[1]).click().focus();
  },
  k40: function (e) {//down. select the next directory
    var B = imce.tree[imce.conf.dir], L = B.li, U = B.ul;
    if (U && imce.hasC(L, 'expanded')) $(U.firstChild.childNodes[1]).click().focus();
    else do {if (L.nextSibling) return $(L.nextSibling.childNodes[1]).click().focus();
    }while ((L = L.parentNode.parentNode).tagName == 'LI');
  }
};
//add equal keys
imce.dirKeys.k36 = imce.dirKeys.k35;
imce.dirKeys.k39 = imce.dirKeys.k37;

//shortcut key-function pairs for files
imce.fileKeys = {
  k38: function (e) {//up-down. select previous-next row
    var fid = imce.lastFid(), i = fid ? imce.fids[fid].rowIndex+e.keyCode-39 : 0;
    imce.fileClick(imce.findex[i], e.ctrlKey, e.shiftKey);
  },
  k35: function (e) {//end-home. select first or last row
    imce.fileClick(imce.findex[e.keyCode == 35 ? imce.findex.length-1 : 0], e.ctrlKey, e.shiftKey);
  },
  k13: function (e) {//enter-insert. send file to external app.
    imce.send(imce.vars.prvfid);
    return false;
  },
  k37: function (e) {//left. focus on directories
    $(imce.tree[imce.conf.dir].a).focus();
  },
  k65: function (e) {//ctrl+A to select all
    if (e.ctrlKey && imce.findex.length) {
      var fid = imce.findex[0].id;
      imce.selected[fid] ? (imce.vars.lastfid = fid) : imce.fileClick(fid);//select first row
      imce.fileClick(imce.findex[imce.findex.length-1], false, true);//shift+click last row
      return false;
    }
  }
};
//add equal keys
imce.fileKeys.k40 = imce.fileKeys.k38;
imce.fileKeys.k36 = imce.fileKeys.k35;
imce.fileKeys.k45 = imce.fileKeys.k13;
//add default operation keys. delete, R(esize), T(humbnails), U(pload)
$.each({k46: 'delete', k82: 'resize', k84: 'thumb', k85: 'upload'}, function (k, op) {
  imce.fileKeys[k] = function (e) {
    if (imce.ops[op] && !imce.ops[op].disabled) imce.opClick(op);
  };
});

/**************** SORTING ********************/

//prepare column sorting
imce.initiateSorting = function() {
  //add cache hook. cache the old directory's sort settings before the new one replaces it.
  imce.hooks.cache.push(function (cache, newdir) {
    cache.cid = imce.vars.cid, cache.dsc = imce.vars.dsc;
  });
  //add navigation hook. refresh sorting after the new directory content is loaded.
  imce.hooks.navigate.push(function (data, olddir, cached) {
    cached ? imce.updateSortState(data.cid, data.dsc) : imce.firstSort();
  });
  imce.vars.cid = imce.cookie('icid')*1;
  imce.vars.dsc = imce.cookie('idsc')*1;
  imce.cols = imce.el('file-header').rows[0].cells;
  $(imce.cols).click(function () {imce.columnSort(this.cellIndex, imce.hasC(this, 'asc'));});
  $(window).unload(function() {imce.cookie('icid', imce.vars.cid); imce.cookie('idsc', imce.vars.dsc ? 1 : 0);});
  imce.firstSort();
};

//sort the list for the first time
imce.firstSort = function() {
  imce.columnSort(imce.vars.cid, imce.vars.dsc);
};

//sort file list according to column index.
imce.columnSort = function(cid, dsc) {
  if (imce.findex.length < 2) return;
  if (cid == imce.vars.cid && dsc != imce.vars.dsc) {
    imce.findex.reverse();
  }
  else {
    var func = 'sort'+ (cid == 0 ? 'Str' : 'Num') + (dsc ? 'Dsc' : 'Asc');
    var prop = cid == 2 || cid == 3 ? 'innerHTML' : 'id';
    //sort rows
    imce.findex.sort(cid ? function(r1, r2) {return imce[func](r1.cells[cid][prop], r2.cells[cid][prop])} : function(r1, r2) {return imce[func](r1.id, r2.id)});
  }
  //insert sorted rows
  for (var row, i=0; row = imce.findex[i]; i++) {
    imce.tbody.appendChild(row);
  }
  imce.updateSortState(cid, dsc);
};

//update column states
imce.updateSortState = function(cid, dsc) {
  $(imce.cols[imce.vars.cid]).removeClass(imce.vars.dsc ? 'desc' : 'asc');
  $(imce.cols[cid]).addClass(dsc ? 'desc' : 'asc');
  imce.vars.cid = cid;
  imce.vars.dsc = dsc;
};

//sorters
imce.sortStrAsc = function(a, b) {return a.toLowerCase() < b.toLowerCase() ? -1 : 1;};
imce.sortStrDsc = function(a, b) {return imce.sortStrAsc(b, a);};
imce.sortNumAsc = function(a, b) {return a-b;};
imce.sortNumDsc = function(a, b) {return b-a};

/**************** RESIZE-BARS  ********************/

//set resizers for resizable areas and recall previous dimensions
imce.initiateResizeBars = function () {
  imce.setResizer('navigation-resizer', 'X', 'navigation-wrapper', null, 1);
  imce.setResizer('browse-resizer', 'Y', 'browse-wrapper', 'preview-wrapper', 50);
  imce.recallDimensions();
  $(window).unload(function() {
    imce.cookie('imcebwh', $(imce.BW).height());
    imce.cookie('imcenww', Math.max($(imce.NW).width(), 1));
  });
};

//set a resize bar
imce.setResizer = function (resizer, axis, area1, area2, Min, callback) {
  var opt = axis == 'X' ? {pos: 'pageX', func: 'width'} : {pos: 'pageY', func: 'height'};
  var Min = Min || 0;
  var $area1 = $(imce.el(area1)), $area2 = area2 ? $(imce.el(area2)) : null, $doc = $(document);
  $(imce.el(resizer)).mousedown(function(e) {
    var pos = e[opt.pos];
    var end = start = $area1[opt.func]();
    var Max = $area2 ? start + $area2[opt.func]() : 1200;
    var drag = function(e) {
      end = Math.min(Max - Min, Math.max(start + e[opt.pos] - pos, Min));
      $area1[opt.func](end);
      $area2 && $area2[opt.func](Max - end);
      return false;
    };
    var undrag = function(e) {
      $doc.unbind('mousemove', drag).unbind('mouseup', undrag);
      callback && callback(start, end, Max);
    };
    $doc.mousemove(drag).mouseup(undrag);
    return false;
  });
};

//get&set area dimensions of the last session from the cookie
imce.recallDimensions = function() {
  var $body = $(document.body);
  if (!$body.is('.imce')) return;
  //row heights
  imce.recallHeights(imce.cookie('imcebwh') * 1);
  $(window).resize(function(){imce.recallHeights()});
  //navigation wrapper
  var nwOldWidth = imce.cookie('imcenww') * 1;
  nwOldWidth && $(imce.NW).width(Math.min(nwOldWidth, $body.width() - 10));
};

//set row heights with respect to window height
imce.recallHeights = function(bwFixedHeight) {
  //window & body dimensions
  var winHeight = $.browser.opera ? window.innerHeight : $(window).height();
  var bodyHeight = $(document.body).outerHeight(true);
  var diff = winHeight - bodyHeight;
  var bwHeight = $(imce.BW).height(), pwHeight = $(imce.PW).height();
  if (bwFixedHeight) {
    //row heights
    diff -= bwFixedHeight - bwHeight;
    bwHeight = bwFixedHeight;
    pwHeight += diff;
  }
  else {
    diff = parseInt(diff/2);
    bwHeight += diff;
    pwHeight += diff;
  }
  $(imce.BW).height(bwHeight);
  $(imce.PW).height(pwHeight);
};

//cookie get & set
imce.cookie = function (name, value) {
  if (typeof(value) == 'undefined') {//get
    return unescape((document.cookie.match(new RegExp('(^|;) *'+ name +'=([^;]*)(;|$)')) || ['', '', ''])[2]);
  }
  document.cookie = name +'='+ escape(value) +'; expires='+ (new Date(new Date()*1 + 30*86400000)).toGMTString() +'; path=/';//set
};

//view thumbnails(smaller than tMaxW x tMaxH) inside the rows.
//Large images can also be previewed by setting imce.vars.prvstyle to a valid image style(imagecache preset)
imce.thumbRow = function (row) {
  var w = row.cells[2].innerHTML * 1;
  if (!w) return;
  var h = row.cells[3].innerHTML*1;
  if (imce.vars.tMaxW < w || imce.vars.tMaxH < h) {
    if (!imce.vars.prvstyle) return;
    var img = new Image();
    img.src = imce.imagestyleURL(imce.getURL(row.id), imce.vars.prvstyle);
    img.className = 'imagestyle imagestyle-' + imce.vars.prvstyle;
  }
  else {
    var prvH = h, prvW = w;
    if (imce.vars.prvW < w || imce.vars.prvH < h) {
      if (h < w) {
        prvW = imce.vars.prvW;
        prvH = prvW*h/w;
      }
      else {
        prvH = imce.vars.prvH;
        prvW = prvH*w/h;
      }
    }
    var img = new Image(prvW, prvH);
    img.src = imce.getURL(row.id);
  }
  var cell = row.cells[0];
  cell.insertBefore(img, cell.firstChild);
};

//convert a file URL returned by imce.getURL() to an image style(imagecache preset) URL
imce.imagestyleURL = function (url, stylename) {
  var len = imce.conf.furl.length - 1;
  return url.substr(0, len) + '/imagecache/' + stylename + url.substr(len);
};

})(jQuery);