const packet = [
  {
    title: '계정관련',
  },
  {
    name: 'reg_id',
    cmd: 101,
    server: 1,
    comment: '계정생성',
    updatedt: '2022-07-28',
    updatemsg: '패킷 추가',
  },
  {
    name: 'login',
    cmd: 103,
    server: 1,
    comment: '로그인',
    updatedt: '2022-08-14',
    updatemsg: '패킷 수정',
  },
  {
    name: 'agreement',
    cmd: 105,
    server: 1,
    comment: '여러 약관 동의',
    updatedt: '2022-08-14',
    updatemsg: '패킷 추가',
  },
  {
    name: 'account_switching',
    cmd: 113,
    server: 1,
    comment: 'SNS계정 전환',
    updatedt: '2022-07-28',
    updatemsg: '패킷 추가(미생성)',
  },
  {
    name: 'api_token_reflush',
    cmd: 115,
    server: 1,
    comment: 'API 토큰 갱신',
    updatedt: '2022-07-28',
    updatemsg: '패킷 추가(미생성)',
  },
  {
    name: 'item_list',
    cmd: 201,
    server: 1,
    comment: '아이템 리스트',
    updatedt: '2022-07-28',
    updatemsg: '패킷 추가',
  },
  {
    name: 'get_exp',
    cmd: 203,
    server: 1,
    comment: '경험치 확인',
    updatedt: '2022-07-28',
    updatemsg: '패킷 추가',
  },
  {
    name: 'unit_list',
    cmd: 301,
    server: 1,
    comment: '유닛 리스트',
    updatedt: '2022-07-28',
    updatemsg: '패킷 추가',
  },
  {
    name: 'unit_trans',
    cmd: 303,
    server: 1,
    comment: '아이템=>유닛 교환',
    updatedt: '2022-07-31',
    updatemsg: '패킷 추가',
  },
];

export default packet;