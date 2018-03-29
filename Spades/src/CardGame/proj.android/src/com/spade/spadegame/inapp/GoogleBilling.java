package com.spade.spadegame.inapp;

import java.util.ArrayList;

import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

import com.android.vending.billing.IInAppBillingService;
import com.spade.spadegame.GoogleAdvertise;
import com.spade.spadegame.fb.FacebookConnect;
import com.spade.spadegame.fb.FacebookScore;

public class GoogleBilling {

	private static final String TAG = "GoogleService";
	private static final int REQ_GOOGLE_PURCHASE = 1001;

	private static final int MSG_PURCHASE_ADS = 100;
	private static final int MSG_PURCHASE_WIN = 101;
	private static final int MSG_PURCHASE_LOSE = 102;

	private static final String PRODUCT_CODE_ADS = "remove_ads_399";
	private static final String PRODUCT_CODE_WIN = "purchase_wins_10";
	private static final String PRODUCT_CODE_LOSE = "purchase_loses_10";

	private static final String RESPONSE_INAPP_ITEM_LIST = "INAPP_PURCHASE_ITEM_LIST";
	private static final String RESPONSE_INAPP_PURCHASE_DATA_LIST = "INAPP_PURCHASE_DATA_LIST";

	private static GoogleBilling instance;
	private Activity activity;

	IInAppBillingService mService;

	ServiceConnection mServiceConn = new ServiceConnection() {
		@Override
		public void onServiceDisconnected(ComponentName name) {
			mService = null;
		}

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mService = IInAppBillingService.Stub.asInterface(service);

			if (!GoogleAdvertise.isDisableAdver(activity)) {
				try{
					queryPurchase();
				} catch(Exception e) {
					Log.e(TAG, "QueryPurchase exception err=" + e.toString());
				}
			}
		}
	};

	private static final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_PURCHASE_ADS:
				instance.purchaseAction(PRODUCT_CODE_ADS, false);
				break;
			case MSG_PURCHASE_WIN:
				instance.purchaseAction(PRODUCT_CODE_WIN, true);
				break;
			case MSG_PURCHASE_LOSE:
				instance.purchaseAction(PRODUCT_CODE_LOSE, true);
				break;
			default:
				break;
			}
		}
	};

	public GoogleBilling(Activity activity) {
		this.activity = activity;
		instance = this;
	}

	public void release() {
		instance = null;
	}

	public void startService() {
		Intent serviceIntent = new Intent("com.android.vending.billing.InAppBillingService.BIND");
		serviceIntent.setPackage("com.android.vending");
		activity.bindService(serviceIntent, mServiceConn, Context.BIND_AUTO_CREATE);
	}

	public void stopService() {
		if (mService != null) {
			activity.unbindService(mServiceConn);
		}
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch(requestCode) {
		case REQ_GOOGLE_PURCHASE:
		{
			String purchaseData = data.getStringExtra("INAPP_PURCHASE_DATA");
			Log.d(TAG, "onActivityResult purchaseData=" + purchaseData);

			if (resultCode == Activity.RESULT_OK) {
				try {
					JSONObject jo = new JSONObject(purchaseData);
					String sku = jo.getString("productId");

					Log.d(TAG, "onActivityResult sku=" + sku);

					if (sku.equals(PRODUCT_CODE_ADS)) {
						onPurchaseResult(1, true);
						return;
					} else if (sku.equals(PRODUCT_CODE_WIN)) {
						onPurchaseResult(2, true);
						return;
					} else if (sku.equals(PRODUCT_CODE_LOSE)) {
						onPurchaseResult(3, true);
						return;
					}
				} catch (JSONException e) {
					Log.e(TAG, "Failed to parse purchase data.", e);
				}
			}

			onPurchaseResult(0, false);
		}
		break;
		}
	}

	public void purchaseAction(String purchase, boolean consume) {
		Log.d(TAG, "purchaseAction item_code=" + purchase);

		if (mService == null) {
			Toast.makeText(activity, "Cannot connect to Google play service...", Toast.LENGTH_SHORT).show();
		} else {
			try {
				if (consume) {
					Log.d(TAG, "consumePurchase");
					consumePurchase(purchase);
				}

				Log.d(TAG, "startIntentSenderForResult");

				Bundle buyIntentBundle = mService.getBuyIntent(3, activity.getPackageName(), purchase, "inapp", null);
				PendingIntent pendingIntent = buyIntentBundle.getParcelable("BUY_INTENT");
				activity.startIntentSenderForResult(pendingIntent.getIntentSender(), REQ_GOOGLE_PURCHASE, new Intent(), Integer.valueOf(0), Integer.valueOf(0), Integer.valueOf(0));

			} catch (Exception e) {
				Log.e(TAG, "purchaseAction exception", e);
				Toast.makeText(activity, "Cannot purchase", Toast.LENGTH_SHORT).show();
			}
		}
	}

	private void onPurchaseResult(int action, boolean success) {
		if (success) {
			int win = FacebookConnect.game.win;
			int lose = FacebookConnect.game.lose;
			Toast.makeText(activity, "Google Purchase sucessed.", Toast.LENGTH_SHORT).show();
			switch (action) {
			case 1:
				GoogleAdvertise.disableAdver();
				break;
			case 2:
				try {
					consumePurchase(PRODUCT_CODE_WIN);
				} catch (Exception e) {
					Log.e(TAG, "consume error", e);
				}
				win += 10;
				FacebookScore.nativeSaveScore(win, lose);
				Cocos2dxHelper.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						nativeCallback(2);
					}
				});
				break;
			case 3:
				try {
					consumePurchase(PRODUCT_CODE_LOSE);
				} catch (Exception e) {
					Log.e(TAG, "consume error", e);
				}
				lose -= 10;
				if (lose < 0) {
					lose = 0;
				}
				FacebookScore.nativeSaveScore(win, lose);
				Cocos2dxHelper.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						nativeCallback(3);
					}
				});
				break;
			}
		} else {
			Toast.makeText(activity, "Google Purchase failed.", Toast.LENGTH_SHORT).show();
		}
	}

	private void queryPurchase() throws RemoteException, JSONException {
		Log.d(TAG, "QueryPurchase s");

		Bundle ownedItems = mService.getPurchases(3, activity.getPackageName(), "inapp", null);
		int response = ownedItems.getInt("RESPONSE_CODE");

		Log.d(TAG, "QueryPurchase response  " + ownedItems.toString());
		if (response == 0) {
			ArrayList<String> ownedSkus = ownedItems.getStringArrayList("INAPP_PURCHASE_ITEM_LIST");

			if (ownedSkus == null){
				Log.e(TAG, "QueryPurchase ownedSkus=null");
			}

			for (int i = 0; i < ownedSkus.size(); ++i) {
				String sku = ownedSkus.get(i);

				Log.d(TAG, "purchasequery, purchaseData="+sku);

				if (sku.equals(PRODUCT_CODE_ADS)) {
					Log.d(TAG, "onPurchaseOK ");
					GoogleAdvertise.disableAdver();
				}
			}
		}

		Log.d(TAG, "QueryPurchase e");
	}

	private void consumePurchase(String productId) throws RemoteException, JSONException {
		Bundle ownedItems = mService.getPurchases(3, activity.getPackageName(), "inapp", null);
		int response = ownedItems.getInt("RESPONSE_CODE");

		Log.d(TAG, "QueryPurchase response  " + ownedItems.toString());
		if (response == 0) {
			ArrayList<String> ownedSkus = ownedItems.getStringArrayList(RESPONSE_INAPP_ITEM_LIST);
			ArrayList<String> purchaseDataList = ownedItems.getStringArrayList(RESPONSE_INAPP_PURCHASE_DATA_LIST);

			if (ownedSkus == null || purchaseDataList == null) {
				Log.e(TAG, "QueryPurchase ownedSkus=null or purchaseDataList = null");
			} else {
				for (int i = 0; i < purchaseDataList.size(); ++i) {
					String sku = ownedSkus.get(i);
					if (sku.equals(productId)) {
						String purchaseData = purchaseDataList.get(i);
						Purchase purchase = new Purchase("inapp", purchaseData);
						mService.consumePurchase(3, activity.getPackageName(), purchase.getToken());
						break;
					}
				}
			}
		}
	}

	public static void nativePurchaseAds() {
		if (!GoogleAdvertise.isDisableAdver(instance.activity)) {
			mHandler.sendEmptyMessage(MSG_PURCHASE_ADS);
		} else {
			Toast.makeText(instance.activity, "Already purchased", Toast.LENGTH_SHORT).show();
		}
	}

	public static void nativePurchaseWin() {
		mHandler.sendEmptyMessage(MSG_PURCHASE_WIN);
	}

	public static void nativePurchaseLose() {
		mHandler.sendEmptyMessage(MSG_PURCHASE_LOSE);
	}

	public static native void nativeCallback(int action);
}
