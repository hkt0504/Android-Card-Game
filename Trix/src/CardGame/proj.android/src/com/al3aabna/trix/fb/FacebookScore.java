package com.al3aabna.trix.fb;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import org.cocos2dx.lib.Cocos2dxHelper;
import org.json.JSONArray;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.al3aabna.trix.R;
import com.al3aabna.trix.entry.Game;
import com.facebook.FacebookRequestError;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.facebook.model.GraphObject;

public class FacebookScore {

	public static final String TAG = "FacebookScore";
	private static final int MSG_GET_SCORE = 10;
	private static final int MSG_SAVE_SCORE = 11;

	private static FacebookScore instance;

	private UiLifecycleHelper uiHelper;
	private Session.StatusCallback statusCallback = new SessionStatusCallback();
	private Activity activity;

	private String fbAppID;
	private boolean running = false;
	private int needAction = 0;
	private int savedScore = 0;
	private ArrayList<Game> games = new ArrayList<Game>();

	public static FacebookScore instance() {
		return instance;
	}

	private static final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_GET_SCORE:
				instance.getScores();
				break;
			case MSG_SAVE_SCORE:
				instance.saveScore(msg.arg1);
				break;
			}
		}
	};

	public FacebookScore(Activity activity) {
		instance = this;
		this.activity = activity;
		fbAppID = activity.getResources().getString(R.string.fb_app_id);

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

	private void getScores() {
		if (running) return;
		final Session session = Session.getActiveSession();

		if (checkFriendPermisson(session)) {
			Request request = Request.newGraphPathRequest(session, fbAppID + "/scores" , new Request.Callback() {
				@Override
				public void onCompleted(Response response) {
					FacebookRequestError error = response.getError();
					if (error != null) {
						Log.e(TAG, error.toString());
					} else if (session == Session.getActiveSession()) {
						if (response != null) {
							try {
								GraphObject graphObject = response.getGraphObject();
								JSONArray dataArray = (JSONArray)graphObject.getProperty("data");

								games.clear(); 
								for (int i=0; i < dataArray.length(); i++) {
									JSONObject oneData = dataArray.optJSONObject(i);
									int score = oneData.optInt("score");

									JSONObject userObj = oneData.optJSONObject("user");
									String userID = userObj.optString("id");
									String userName = userObj.optString("name");

									games.add(new Game(userID, userName, score));
								}

								Comparator<Game> comparator = Collections.reverseOrder();
								Collections.sort(games, comparator);

								Cocos2dxHelper.runOnGLThread(new Runnable() {
									@Override
									public void run() {
										nativeCallback(games);
									}
								});
							} catch (Exception e) {
								Log.e(TAG, "error", e);
							}
						}
					}
					running = false;
				}
			});
			request.executeAsync();
		} else {
			requestFriendsPermission(session);
		}
	}

	private void saveScore(int score) {
		final Session session = Session.getActiveSession();

		if (checkPublishPermisson(session)) {
			postScore(session, score);
		} else {
			requestPublishPermission(session, score);
		}
	}

	private void postScore(final Session session, final int score) {
		Bundle fbParams = new Bundle();
		fbParams.putString("score", "" + score);
		Request request = new Request(session, "me/scores", fbParams, HttpMethod.POST, new Request.Callback() {
			@Override
			public void onCompleted(Response response) {
				FacebookRequestError error = response.getError();
				if (error != null) {
					String msg = error.getErrorMessage();
					if (msg != null)
						Log.e(TAG, "Posting Score to Facebook failed: " + error.getErrorMessage());
					showRetryPopup(session, score);
				} else {
					Log.i(TAG, "Score posted successfully to Facebook");
					FacebookConnect.game.setFacebookScore(score);
				}
			}
		});
		request.executeAsync();
	}

	private void showRetryPopup(final Session session, final int score) {
		AlertDialog.Builder alertBuilder = new AlertDialog.Builder(activity);
		alertBuilder.setTitle("Facebook Error");
		alertBuilder.setMessage("Facebook error occured, If you press cancel, you lose score. Please retry.");
		alertBuilder.setPositiveButton("Retry", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				postScore(session, score);
			}
		});
		alertBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
			}
		});
		alertBuilder.setCancelable(false);
		alertBuilder.create();
		alertBuilder.show();
	}

	private boolean checkFriendPermisson(Session session) {
		return session.getPermissions().contains("user_friends");
	}

	private boolean checkPublishPermisson(Session session) {
		return session.getPermissions().contains("publish_actions");
	}

	private void requestFriendsPermission(Session session) {
		Session.NewPermissionsRequest reqeust = new Session.NewPermissionsRequest(activity, "user_friends");
		session.addCallback(statusCallback);
		session.requestNewReadPermissions(reqeust);
		needAction = MSG_GET_SCORE;
	}

	private void requestPublishPermission(Session session, int score) {
		Session.NewPermissionsRequest request = new Session.NewPermissionsRequest(activity, "publish_actions");
		session.addCallback(statusCallback);
		session.requestNewPublishPermissions(request);
		needAction = MSG_SAVE_SCORE;
		savedScore = score;
	}

	private void onSessionStateChange(Session session, SessionState state, Exception exception) {
		if (state.isOpened()) {
			Log.i(TAG, "Logged in...");
		} else if (state.isClosed()) {
			Log.i(TAG, "Logged out...");
		}

		if (needAction > 0 && state == SessionState.OPENED_TOKEN_UPDATED) {
			int action = needAction;
			needAction = 0;
			if (action == MSG_GET_SCORE) {
				getScores();
			} else {
				postScore(session, savedScore);
			}
		}
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

	public static void nativeGetScores() {
		mHandler.sendEmptyMessage(MSG_GET_SCORE);
	}

	public static void nativeSaveScore(int win, int lose) {
		int score = ((win & 0xFFFF) << 16) | (lose & 0xFFFF);
		Message msg = new Message();
		msg.what = MSG_SAVE_SCORE;
		msg.arg1 = score;
		mHandler.sendMessage(msg);
	}

	public static void nativeWinGame() {
		nativeSaveScore(FacebookConnect.game.win + 1, FacebookConnect.game.lose);
	}

	public static void nativeLoseGame() {
		nativeSaveScore(FacebookConnect.game.win, FacebookConnect.game.lose + 1);
	}

	public static int nativeGetWin() {
		return FacebookConnect.game.win;
	}

	public static int nativeGetLose() {
		return FacebookConnect.game.lose;
	}

	private static native void nativeCallback(ArrayList<Game> games);


	private class SessionStatusCallback implements Session.StatusCallback {
		@Override
		public void call(Session session, SessionState state, Exception exception) {
			// Respond to session state changes, ex: updating the view
			onSessionStateChange(session, state, exception);
		}
	}

}
