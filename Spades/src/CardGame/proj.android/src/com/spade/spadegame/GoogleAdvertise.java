package com.spade.spadegame;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.Message;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.spade.spadegame.fb.FacebookConnect;

public class GoogleAdvertise {

	// Google advertise id
	private static final String ADVER_ID = "ca-app-pub-9147961153943755/6645152425";
	private static final long INTERVAL = 120000; // 2min
	private static final long PROCESSING_TIME = 10000; // 10sec
	private static final int MSG_ADVER = 10;

	InterstitialAd interstitialAd;
	AdRequest adRequest;
	boolean disableAdver = false;

	private static GoogleAdvertise instance;
	private Activity activity;

	private static final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_ADVER:
				instance.showAdver();
				break;
			default:
				break;
			}
		}
	};

	public GoogleAdvertise(Activity activity) {
		this.activity = activity;
		instance = this;
		disableAdver = isDisableAdver(activity);
	}

	public void release() {
		instance = null;
	}

	public void onCreate() {
		if (!disableAdver) {
			interstitialAd = new InterstitialAd(activity);
			interstitialAd.setAdUnitId(ADVER_ID);
			interstitialAd.setAdListener(new AdListener() {
				@Override
				public void onAdLoaded() {
					super.onAdLoaded();
					if (!disableAdver && interstitialAd.isLoaded()) {
						performShow();
					}
				}

				@Override
				public void onAdClosed() {
					super.onAdClosed();

					mHandler.sendEmptyMessageDelayed(MSG_ADVER, INTERVAL);
				}

				@Override
				public void onAdFailedToLoad(int errorCode) {
					super.onAdFailedToLoad(errorCode);
				}
			});

			adRequest = new AdRequest.Builder().build();
			if (isNetworkConnectionAvailable(activity)) {
				interstitialAd.loadAd(adRequest);
			} else {
				mHandler.sendEmptyMessageDelayed(MSG_ADVER, INTERVAL * 2);
			}
		}
	}

	public void onDestroy() {
		removeMessages();
	}

	public static void disableAdver() {
		instance.disableAdver = true;
		instance.removeMessages();
		disableAdver(instance.activity);
	}

	public static boolean isDisableAdver() {
		return instance.disableAdver;
	}

	public static void disableAdver(Context context) {
		SharedPreferences pref = context.getSharedPreferences("spade", Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = pref.edit();
		editor.putBoolean("adver", true);
		editor.commit();
	}

	public static boolean isDisableAdver(Context context) {
		SharedPreferences pref = context.getSharedPreferences("spade", Context.MODE_PRIVATE);
		return pref.getBoolean("adver", false);
	}

	private void performShow() {
		if (FacebookConnect.instance().isProcessing()) {
			mHandler.sendEmptyMessageDelayed(MSG_ADVER, PROCESSING_TIME);
		} else {
			interstitialAd.show();
		}
	}

	private void showAdver() {
		if (!disableAdver) {
			if (interstitialAd.isLoaded()) {
				performShow();
			} else {
				interstitialAd.loadAd(adRequest);
			}
		}
	}

	private void removeMessages() {
		if (mHandler.hasMessages(MSG_ADVER)) {
			mHandler.removeMessages(MSG_ADVER);
		}
	}

	private static boolean isNetworkConnectionAvailable(Context context) {
		if (context != null) {
			ConnectivityManager cm = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo info = cm.getActiveNetworkInfo();
			if (info == null) {
				return false;
			}

			NetworkInfo.State network = info.getState();
			return (network == NetworkInfo.State.CONNECTED || network == NetworkInfo.State.CONNECTING);
		}

		return false;
	}
}
