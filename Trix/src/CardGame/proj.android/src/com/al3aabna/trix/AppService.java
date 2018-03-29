package com.al3aabna.trix;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.database.ContentObserver;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Handler;

public class AppService {

	private static AppService instance;
	private Activity activity;
	private SettingsContentObserver volumeListener;

	public static AppService init(Activity activity) {
		instance = new AppService(activity);
		return instance;
	}

	AppService(Activity activity) {
		this.activity = activity;
		volumeListener = new SettingsContentObserver(activity, new Handler());
	}

	public void release() {
		instance = null;
	}

	public void onCreate() {
		activity.getContentResolver().registerContentObserver(android.provider.Settings.System.CONTENT_URI, true, volumeListener);
	}

	public void onDestroy() {
		if (volumeListener != null) {
			activity.getContentResolver().unregisterContentObserver(volumeListener);
			volumeListener = null;
		}
	}

	private void openMoreGame() {
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setData(Uri.parse("https://play.google.com/store/apps/developer?id=Innovative+Applications"));
		activity.startActivity(i);
	}

	private void openRateGame(){
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setData(Uri.parse("https://play.google.com/store/apps/details?id=com.spade.spadegame"));
		activity.startActivity(i);
	}

	private void muteVolume() {
		AudioManager audioManager = (AudioManager) activity.getSystemService(Context.AUDIO_SERVICE);
		audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, 0, AudioManager.FLAG_PLAY_SOUND);
	}

	private void unmuteVolume() {
		AudioManager audioManager = (AudioManager) activity.getSystemService(Context.AUDIO_SERVICE);
		int maxVol = audioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		audioManager.setStreamVolume(AudioManager.STREAM_MUSIC, maxVol / 2, AudioManager.FLAG_PLAY_SOUND);
	}

	private boolean isMuteVolume() {
		AudioManager audioManager = (AudioManager) activity.getSystemService(Context.AUDIO_SERVICE);
		int volume = audioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
		return (volume == 0);
	}

	private void onMuteChanged(final boolean mute) {
		Cocos2dxHelper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				nativeSoundCallback(mute);
			}
		});
	}

	public static void nativeOpenMoreGame() {
		instance.openMoreGame();
	}

	public static void nativeOpenRateGame() {
		instance.openRateGame();
	}

	public static void nativeMuteVolume() {
		instance.muteVolume();
	}

	public static void nativeUnmuteVolume() {
		instance.unmuteVolume();
	}

	public static boolean nativeIsMuteVolume() {
		return instance.isMuteVolume();
	}

	public static native void nativeSoundCallback(boolean mute);

	public static class SettingsContentObserver extends ContentObserver {
		boolean prevMute;
		Context context;

		public SettingsContentObserver(Context c, Handler handler) {
			super(handler);
			context = c;

			AudioManager audio = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
			prevMute = audio.getStreamVolume(AudioManager.STREAM_MUSIC) == 0;
		}

		@Override
		public boolean deliverSelfNotifications() {
			return super.deliverSelfNotifications();
		}

		@Override
		public void onChange(boolean selfChange) {
			super.onChange(selfChange);

			AudioManager audio = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
			boolean curMute = audio.getStreamVolume(AudioManager.STREAM_MUSIC) == 0;

			if (instance != null && curMute != prevMute) {
				instance.onMuteChanged(curMute);
			}
			prevMute = curMute;
		}

	}
}
