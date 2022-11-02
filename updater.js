const { autoUpdater } = require('electron-updater');
const { dialog } = require('electron');
const ginfo = require('./src/ginfo');

exports.check = function (update_url) {
  return new Promise((resolve) => {
    autoUpdater.autoInstallOnAppQuit = false;
    //autoUpdater.allowDowngrade = true;

    if (!update_url) return resolve();

    autoUpdater.setFeedURL(update_url);

    autoUpdater.addListener('checking-for-update', () => {
      ginfo.console_message('업데이트를 체크중입니다...');
    });

    autoUpdater.addListener('update-available', (ev) => {
      ginfo.console_message('새로운 업데이트를 발견했습니다.');
      ginfo.console_message('version : ' + ev.version);
      ginfo.console_message('path : ' + ev.path);
      ginfo.console_message('update date : ' + ev.releaseDate);
    });

    autoUpdater.addListener('download-progress', (p) => {
      let msg = 'down speed : ' + p.bytesPerSecond;
      msg = msg + ' (' + p.transferred + '/' + p.total + ')';
      ginfo.console_message(msg);
    });

    autoUpdater.addListener('update-not-available', (ev) => {
      ginfo.console_message('최신버전입니다. version : ' + ev.version);
      resolve();
    });

    autoUpdater.addListener('error', (err) => {
      ginfo.console_message('업데이트 오류 : ' + err);
      resolve();
    });

    autoUpdater.addListener('update-downloaded', (ev, releaseNotes, releaseName) => {
      ginfo.console_message('Update downloaded');
      const dialogOpts = {
        type: 'info',
        buttons: ['Restart', 'Later'],
        title: 'Application Update',
        message: process.platform === 'win32' ? releaseNotes : releaseName,
        detail: '새로운 버전이 다운로드 되었습니다. 업데이트를 적용하기 위해 앱을 재시작하세요.',
      };

      dialog.showMessageBox(dialogOpts).then((returnValue) => {
        if (returnValue.response === 0) {
          ginfo.console_message('업데이트 시작');
          autoUpdater.quitAndInstall();
        } else {
          ginfo.console_message('업데이트 취소');
        }

        resolve();
      });
    });

    // tell squirrel to check for updates
    autoUpdater.checkForUpdates();
  });
};
