package com.spade.spadegame.fb;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import com.facebook.FacebookException;
import com.facebook.FacebookRequestError;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.spade.spadegame.R;
import com.spade.spadegame.entry.Game;

public class FacebookConnect {

	private static final String TAG = "FacebookConnect";

	private static FacebookConnect instance;

	private static final int MSG_LOGIN = 10;
	private static final int MSG_LOGOUT = 11;
	private static final int MSG_LOGINED = 12;

	private UiLifecycleHelper uiHelper;
	private Session.StatusCallback statusCallback = new SessionStatusCallback();
	private Activity activity;

	private Session fbSession = null;
	private boolean isLogined = false;
	private boolean isProcessing = false;
	private boolean needLogin = false;

	private String userImagePath = null;
	public static Game game = new Game();
	private static int action;

	private static final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_LOGIN:
				FacebookConnect.action = msg.arg1;
				instance.performLogin();
				break;
			case MSG_LOGOUT:
				FacebookConnect.action = msg.arg1;
				instance.performLogout();
				break;
			case MSG_LOGINED:
				Cocos2dxHelper.runOnGLThread(new Runnable() {
					@Override
					public void run() {
						nativeCallback(action, game);
					}
				});
				break;
			}
		}
	};

	public FacebookConnect(Activity activity) {
		this.activity = activity;
		instance = this;
		try {
			uiHelper = new UiLifecycleHelper(activity, statusCallback);
		} catch (Error e) {
			Log.e(TAG, "LifecycleHelper Error", e);
		} catch (Exception e) {
			Log.e(TAG, "LifecycleHelper Error", e);
		}
	}

	public void release() {
		instance = null;
	}

	public static FacebookConnect instance() {
		return instance;
	}

	public boolean isProcessing() {
		return isProcessing;
	}

	public void onCreate(Bundle savedInstanceState) {
		if (uiHelper != null) {
			uiHelper.onCreate(savedInstanceState);
		}
	}

	public void onResume() {
		Session session = Session.getActiveSession();
		if (session != null && (session.isOpened() || session.isClosed())) {
			onSessionStateChange(session, session.getState(), null);
		}

		if (uiHelper != null) {
			uiHelper.onResume();
		}
	}

	public void onPause() {
		if (uiHelper != null) {
			uiHelper.onPause();
		}
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (uiHelper != null) {
			uiHelper.onActivityResult(requestCode, resultCode, data);
		}
		Session.getActiveSession().onActivityResult(activity, requestCode, resultCode, data);
	}

	public void onSaveInstanceState(Bundle outState) {
		if (uiHelper != null) {
			uiHelper.onSaveInstanceState(outState);
		}
	}

	public void onDestroy() {
		if (uiHelper != null) {
			uiHelper.onDestroy();
		}
	}

	private void performLogin() {
		try {
			isProcessing = true;
			needLogin = true;
			fbSession = Session.getActiveSession();
			if (fbSession == null || !fbSession.isOpened()) {
				fbSession = new Session.Builder(activity).setApplicationId(activity.getString(R.string.fb_app_id)).build();
				Session.setActiveSession(fbSession);
				fbSession.addCallback(statusCallback);
				Session.OpenRequest openRequest = new Session.OpenRequest(activity);
				openRequest.setPermissions("user_friends", "user_games_activity");
				fbSession.openForRead(openRequest);
			} else {
				Session.openActiveSession(activity, true, statusCallback);
			}
		} catch (FacebookException e) {
			Log.e(TAG, "Facebook Error", e);
			isProcessing = false;
			needLogin = false;
			Toast.makeText(activity, "Cannot connect to Facebook", Toast.LENGTH_SHORT).show();
		}
	}

	private void performLogout() {
		isLogined = false;
		game.userId = null;
		game.userName = null;
		Session session = Session.getActiveSession();
		if (session != null && !session.isClosed()) {
			session.closeAndClearTokenInformation();
		}
	}

	private void processUserInfo(Session session) {
//		Request request = Request.newMeRequest(session, new Request.GraphUserCallback() {
//			@Override
//			public void onCompleted(GraphUser user,Response response) {
//				FacebookRequestError error = response.getError();
//				if (user == null || error != null) {
//					Toast.makeText(activity, "Cannot connect to Facebook", Toast.LENGTH_SHORT).show();
//				} else {
//					game.userId = user.getId();
//					game.userName = user.getName();
//					isLogined = true;
//					Log.i(TAG,"User ID "+ game.userId);
//					Log.i(TAG,"User Name "+ user.getName());
//					Cocos2dxHelper.runOnGLThread(new Runnable() {
//						@Override
//						public void run() {
//							nativeCallback(action,"");
//						}
//					});
//				}
//				isProcessing = false;
//			}
//		});
//		request.executeAsync();

		Request request = new Request(session, "me/scores", null, HttpMethod.GET, new Request.Callback() {
			@Override
			public void onCompleted(Response response) {
				FacebookRequestError error = response.getError();
				if (error == null) {
					JSONObject json = response.getGraphObject().getInnerJSONObject();
					try {
						Log.d(TAG, json.toString());
						JSONArray datas = json.getJSONArray("data");
						JSONObject data = datas.getJSONObject(0);
						JSONObject user = data.getJSONObject("user");
						game.userId = user.getString("id");
						game.userName = user.getString("name");

						JSONObject application = data.getJSONObject("application");
						if (application.getString("id").equals(activity.getString(R.string.fb_app_id))) {
							int score = data.optInt("score");
							game.win = (score >> 16) & 0xFFFF;
							game.lose = score & 0xFFFF;
						} else {
							game.win = 0;
							game.lose = 0;
						}

						loadUserImg(game.userId);
					} catch (JSONException e) {
						Log.e(TAG, "error", e);
						Toast.makeText(activity, "Cannot connect to Facebook", Toast.LENGTH_SHORT).show();
					}
				} else {
					Log.e(TAG, "error " + error.getErrorMessage());
					Toast.makeText(activity, "Cannot connect to Facebook", Toast.LENGTH_SHORT).show();
				}
				isProcessing = false;
			}
		});
		request.executeAsync();
	}

	private void loadUserImg(final String userId) {
		Thread thread = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					URL url = new URL(String.format("https://graph.facebook.com/%s/picture?type=small", userId));
				
					HttpURLConnection connection = (HttpURLConnection) url.openConnection();
					HttpURLConnection.setFollowRedirects(HttpURLConnection.getFollowRedirects());
					connection.setDoInput(true);
					connection.connect();

					InputStream input = connection.getInputStream();
			
					BitmapFactory.Options options = new BitmapFactory.Options();
					options.inSampleSize = 1;
					options.inJustDecodeBounds = false;
					options.inPreferredConfig = Config.RGB_565;

					Bitmap bitmap = BitmapFactory.decodeStream(input, null, options);

					String fileName = activity.getFilesDir().getAbsolutePath() + "/" + userId + ".png";
					File bitmapFile = new File(fileName);
					FileOutputStream bitmapWriter = new FileOutputStream(bitmapFile);
					bitmap.compress(Bitmap.CompressFormat.JPEG, 90, bitmapWriter);

					bitmap.recycle();
					userImagePath = fileName;
				} catch (MalformedURLException e) {
					Log.e(TAG, "err", e);
					userImagePath = null;
				} catch (FileNotFoundException e) {
					Log.e(TAG, "err", e);
					userImagePath = null;
				} catch (IOException e) {
					Log.e(TAG, "err", e);
					userImagePath = null;
				} catch (Exception e) {
					Log.e(TAG, "err", e);
					userImagePath = null;
				}

				isLogined = true;
				mHandler.sendEmptyMessage(MSG_LOGINED);
			}
		});

		thread.start();
	}

	private void onSessionStateChange(Session session, SessionState state, Exception exception) {
		if (state.isOpened()) {
			Log.i(TAG, "Logged in...");
			if (needLogin) {
				processUserInfo(session);
				needLogin = false;
			} else {
				isProcessing = false;
			}
		} else if (state.isClosed()) {
			Log.i(TAG, "Logged out...");
			isProcessing = false;
		} else {
			isProcessing = false;
		}
	}

	private class SessionStatusCallback implements Session.StatusCallback {
		@Override
		public void call(Session session, SessionState state, Exception exception) {
			// Respond to session state changes, ex: updating the view
			onSessionStateChange(session, state, exception);
		}
	}

	public static boolean nativeIsLogined() {
		return instance.isLogined;
	}

	public static void nativeLogin(int action) {
		instance.isProcessing = true;

		Message msg = new Message();
		msg.what = MSG_LOGIN;
		msg.arg1 = action;
		mHandler.sendMessage(msg);
	}

	public static void nativeLogout(int action) {
		Message msg = new Message();
		msg.what = MSG_LOGOUT;
		msg.arg1 = action;
		mHandler.sendMessage(msg);
	}

	public static String nativeGetUserId() {
		return game.userId;
	}

	public static String nativeGetUserName() {
		return game.userName == null ? game.userId : game.userName;
	}

	public static String nativeGetUserImage() {
		if (instance.userImagePath != null)
			Log.d(TAG, instance.userImagePath);
		return instance.userImagePath;
	}

	public static native void nativeCallback(int cbIndex, Object params);
}
