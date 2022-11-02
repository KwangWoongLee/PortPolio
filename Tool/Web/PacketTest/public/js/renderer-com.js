/*********************************************************************
 * 현재날짜 얻어오기 YYYY-MM-DD
 **********************************************************************/

function now_date(add_day) {
  var date = new Date();
  if (add_day != undefined) {
    date.setDate(date.getDate() + add_day);
  }
  var year = date.getFullYear();
  var month = date.getMonth() + 1;
  var day = date.getDate();
  if (month < 10) {
    month = '0' + month;
  }
  if (day < 10) {
    day = '0' + day;
  }

  return (today = year + '-' + month + '-' + day);
}

/*********************************************************************
 * email_check
 * email 형식의 텍스트인지 체크
 **********************************************************************/
function email_check(str) {
  var exptext = /^([0-9a-zA-Z_\.-]+)@([0-9a-zA-Z_-]+)(\.[0-9a-zA-Z_-]+){1,2}$/;
  if (!exptext.test(str)) {
    return false;
  } else {
    return true;
  }
}

/*********************************************************************
 * getUrlParams
 * 주소로 넘어오는 query string 을 json 형태로 파싱해서 리턴
 **********************************************************************/
function getUrlParams() {
  var params = {};
  window.location.search.replace(/[?&]+([^=&]+)=([^&]*)/gi, function (str, key, value) {
    params[key] = value;
  });
  return params;
}
