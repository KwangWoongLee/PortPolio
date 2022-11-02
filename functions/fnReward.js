'use strict';

const ref = require('reference');
const mysql = require('db/mysql');
const com = require('com');
const ginfo = require('com/ginfo');
const { error } = require('com/code');

class reward {
  constructor(val) {
    this.session_value = val.session_value;
    this.log = val.log;
    this.modify = false;
    this.add_list = [];
    this.use_list = [];
    this.sort_no = 0;
    this.modify_items = {};
  }

  add_reward_data(reward_code) {
    const ret = ref.reward.get_reward(reward_code);
    for (let i = 0; i < ret.length; i++) {
      const code = ret[i].item_code;
      const cnt = ret[i].item_cnt;

      const err = this.add(code, cnt);
      if (err) return err;
    }

    return null;
  }

  add(code, count, iidx = 0) {
    if (code == 0) return null;
    if (count < 1) return null;

    let prev_cnt = 0;
    let type = '';
    const item_obj = ref.item.get(code);
    if (!item_obj) return error.not_found_ref_item_code;

    let merge_obj = {};

    // 자동으로 열리는 박스
    if (item_obj.type == ginfo.item_type.box_a_random || item_obj.type == ginfo.item_type.box_a_all) {
      for (let i = 0; i < count; i++) {
        this.add_reward_data(item_obj.reward_code);
      }
      return null;
    } else if (item_obj.code == ginfo.money_code.gold) {
      // add gold
      prev_cnt = this.session_value.user.money.gold;
      this.session_value.user.money.gold += count;
      this.money_update = true;
      type = 'G';
    }  else if (item_obj.overlap == 1) {
      // item
      prev_cnt = this.session_value.items[code] == undefined ? 0 : this.session_value.items[code].cnt;
      this.session_value.items[code] = { code, cnt: prev_cnt };
      this.session_value.items[code].cnt += count;
      type = 'I';
    }  else return error.unknown;

    const cur_cnt = prev_cnt + count;

    const add_object = _.merge({ sort_no: this.sort_no++, code, prev_cnt, cur_cnt, add: count, type }, merge_obj);


    this.add_list.push(add_object);
    this.modify = true;

    return null;
  }

  use(code, value) {
    // unique item 의 경우 value = item idx , 이외에는 count
    if (value < 1) return null;
    let prev_cnt = 0;
    let cur_cnt = 0;
    let type = '';

    const item_obj = ref.item.get(code);
    if (!item_obj) return error.not_found_ref_item_code;

    if (item_obj.code == ginfo.money_code.gold) {
      // use gold
      prev_cnt = this.session_value.user.money.gold;
      cur_cnt = prev_cnt - value;
      if (cur_cnt < 0) return error.enough_gold;
      this.session_value.user.money.gold -= value;
      this.money_update = true;
      type = 'G';
    } else {
      // item
      prev_cnt = this.session_value.items[code] == undefined ? 0 : this.session_value.items[code].cnt;
      cur_cnt = prev_cnt - value;
      if (cur_cnt < 0) return error.enough_item;
      if (cur_cnt == 0) _.unset(this.session_value.items, code);
      else this.session_value.items[code].cnt = cur_cnt;

      this.modify_items[code] = cur_cnt;
      type = 'I';
    } 

    const use_object = { sort_no: this.sort_no++, code, prev_cnt, cur_cnt, sub: value, type };
    this.use_list.push(use_object);
    this.modify = true;

    return null;
  }

  get() {
    return {
      add_list: this.add_list.length ? this.add_list : undefined,
      use_list: this.use_list.length ? this.use_list : undefined,
    };
  }
}

exports.new_reward = function (session_value) {
  return new reward(session_value);
};

exports.set_reward = async function (val, reward) {
  const userdb = await mysql.userdb_connect(val);
  await mysql.userdb_transaction(val);

  for (const code in reward.modify_items) {
    const cnt = reward.modify_items[code];
    if (cnt == 0) await del_item(code);
    else await set_item(code, cnt);
  }

  for (let i = 0; i < reward.add_list.length; i++) {
    const obj = reward.add_list[i];
    switch (obj.type) {
      case 'G':
        reward.log.add('add_gold', [obj.cur_cnt, obj.add]);
        break;
      case 'I':
        reward.log.add('add_item', [obj.code, obj.cur_cnt, obj.add]);
        break;
    }
  }

  for (let i = 0; i < reward.use_list.length; i++) {
    const obj = reward.use_list[i];
    switch (obj.type) {
      case 'G':
        reward.log.add('use_gold', [obj.cur_cnt, obj.sub]);
        break;
      case 'I':
        reward.log.add('use_item', [obj.code, obj.cur_cnt, obj.sub]);
        break;
    }
  }

  async function set_item(code, cur_cnt) {
    const exist = await select(code);
    if (exist) await update(code, cur_cnt);
    else await insert(code, cur_cnt);

    function select() {
      return new Promise((resolve, reject) => {
        const query = 'select idx from dat_item where aidx = ? and item_code = ?;';
        mysql
          .query(userdb, query, [val.aidx, code])
          .then((results) => resolve(results.length))
          .catch((e) => reject(e));
      });
    }

    function insert() {
      const query = com.make_query(
        ['insert into dat_item(idx, aidx, item_code, cnt) ', 'values(null, ?, ?, ?);'],
        [val.aidx, code, cur_cnt]
      );
      return mysql.query(userdb, query);
    }

    function update() {
      const query = com.make_query(
        ['update dat_item set cnt = ? where aidx = ? and item_code = ?;'],
        [cur_cnt, val.aidx, code]
      );
      return mysql.query(userdb, query);
    }
  }


  function del_item(code) {
    const query = com.make_query(['delete from dat_item where aidx = ? and item_code = ?; '], [val.aidx, code]);
    return mysql.query(userdb, query);
  }

};
