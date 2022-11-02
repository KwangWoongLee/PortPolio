let win;
module.exports = {
  init: (w) => {
    win = w;
  },
  debug_message: (msg) => {
    console.log(msg);
  },
  console_message: (msg) => {
    win.webContents.send('console-message', msg);
    console.log(msg);
  },
};
