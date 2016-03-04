/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxHelper.Cocos2dxHelperListener;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Message;
import android.view.ViewGroup;
import android.telephony.TelephonyManager;
import android.util.Base64;
import android.util.Log;
import android.widget.FrameLayout;
import android.preference.PreferenceManager.OnActivityResultListener;

import android.net.ConnectivityManager;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.DisplayMetrics;
import android.view.Window;
import android.view.WindowManager;

import com.oasis.sdk.OASISPlatform;
import com.oasis.sdk.OASISPlatformConstant;
import com.oasis.sdk.OASISPlatformInterface;
import com.oasis.sdk.OasisCallback;
import com.oasis.sdk.base.entity.FBPageInfo;
import com.oasis.sdk.base.entity.FriendInfo;
import com.oasis.sdk.base.entity.UserInfo;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public abstract class Cocos2dxActivity extends Activity implements Cocos2dxHelperListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private final static String TAG = Cocos2dxActivity.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================
	
	private Cocos2dxGLSurfaceView mGLSurfaceView;
	private Cocos2dxHandler mHandler;	
	private static Cocos2dxActivity sContext = null;
	private Cocos2dxVideoHelper mVideoHelper = null;
	
	String uri;
	static String ACTION_GAME_SERVER = "game_receiver";

	public static Context getContext() {
		return sContext;
	}
	
	protected void onLoadNativeLibraries() {
		try {
			ApplicationInfo ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
			Bundle bundle = ai.metaData;
			String libName = bundle.getString("android.app.lib_name");
    		System.loadLibrary(libName);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	// ===========================================================
	// Constructors
	// ===========================================================
	
	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		onLoadNativeLibraries();

		sContext = this;
    	this.mHandler = new Cocos2dxHandler(this);
    	
    	Cocos2dxHelper.init(this);
    	Cocos2dxNetwork.init(this);
    	
    	Cocos2dxBroadcastReceiver receiver = new Cocos2dxBroadcastReceiver();
    	IntentFilter filter = getFilter();
    	registerReceiver(receiver, filter);
        
    	this.init();
    	if (mVideoHelper == null) {
    		mVideoHelper = new Cocos2dxVideoHelper(this, mFrameLayout);
		}
    	
		OASISPlatform.init(this);
		
		OASISPlatform.setOASISPlatformInterfaceImpl(new OasisInterfaceImpl());

		OASISPlatform.trackOnCreate(this);

		getFBKeyHash();

		if(getIntent() != null){
			uri = getIntent().getDataString();
		}
	}
	
	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	protected void onResume() {
		super.onResume();

		Cocos2dxHelper.onResume();
		this.mGLSurfaceView.onResume();
		
		OASISPlatform.trackOnResume(this);
	}

	@Override
	protected void onStart() {
		super.onStart();
		OASISPlatform.trackOnStart(this);
	}
	@Override
	protected void onRestart() {
		super.onRestart();
		OASISPlatform.trackOnRestart(this);
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		
		Cocos2dxHelper.onPause();
		this.mGLSurfaceView.onPause();
		
		OASISPlatform.trackOnPause(this);
	}
	
	@Override
	protected void onStop() {
		OASISPlatform.trackOnStop(this);
		super.onStop();
	}
	
	@Override
	protected void onDestroy() {
		OASISPlatform.trackOnDestroy(this);
		OASISPlatform.destroy(this);
		
		super.onDestroy();
	}

	@Override
	public void showDialog(final String pTitle, final String pMessage) {
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_DIALOG;
		msg.obj = new Cocos2dxHandler.DialogMessage(pTitle, pMessage);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) { 
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG;
		msg.obj = new Cocos2dxHandler.EditBoxMessage(pTitle, pContent, pInputMode, pInputFlag, pReturnType, pMaxLength);
		this.mHandler.sendMessage(msg);
	}
	
	@Override
	public void runOnGLThread(final Runnable pRunnable) {
		this.mGLSurfaceView.queueEvent(pRunnable);
	}
	
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        for (OnActivityResultListener listener : Cocos2dxHelper.getOnActivityResultListeners()) {
            listener.onActivityResult(requestCode, resultCode, data);
        }

        super.onActivityResult(requestCode, resultCode, data);
    }


	protected FrameLayout mFrameLayout = null;
	// ===========================================================
	// Methods
	// ===========================================================
	public void init() {
		
    	// FrameLayout
        ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                                       ViewGroup.LayoutParams.MATCH_PARENT);
        mFrameLayout = new FrameLayout(this);
        mFrameLayout.setLayoutParams(framelayout_params);

        // Cocos2dxEditText layout
        ViewGroup.LayoutParams edittext_layout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                                       ViewGroup.LayoutParams.WRAP_CONTENT);
        Cocos2dxEditText edittext = new Cocos2dxEditText(this);
        edittext.setLayoutParams(edittext_layout_params);

        // ...add to FrameLayout
        mFrameLayout.addView(edittext);

        // Cocos2dxGLSurfaceView
        this.mGLSurfaceView = this.onCreateView();

        // ...add to FrameLayout
        mFrameLayout.addView(this.mGLSurfaceView);

        // Switch to supported OpenGL (ARGB888) mode on emulator
        if (isAndroidEmulator())
           this.mGLSurfaceView.setEGLConfigChooser(8 , 8, 8, 8, 16, 0);

        this.mGLSurfaceView.setCocos2dxRenderer(new Cocos2dxRenderer());
        this.mGLSurfaceView.setCocos2dxEditText(edittext);

        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        // Set framelayout as the content view
		setContentView(mFrameLayout);
		

	}
	
    public Cocos2dxGLSurfaceView onCreateView() {
    	return new Cocos2dxGLSurfaceView(this);
    }

   private final static boolean isAndroidEmulator() {
      String model = Build.MODEL;
      Log.d(TAG, "model=" + model);
      String product = Build.PRODUCT;
      Log.d(TAG, "product=" + product);
      boolean isEmulator = false;
      if (product != null) {
         isEmulator = product.equals("sdk") || product.contains("_sdk") || product.contains("sdk_");
      }
      Log.d(TAG, "isEmulator=" + isEmulator);
      return isEmulator;
   }

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
    
    // ===========================================================
    // bbf
    // ===========================================================
   private static final String[] ACTIONS =
	   {ACTION_GAME_SERVER, ConnectivityManager.CONNECTIVITY_ACTION };
   
   private IntentFilter getFilter()
   {
       IntentFilter filter = new IntentFilter();
       for (String action : ACTIONS)
       {
           filter.addAction(action);
       }
       return filter;
   }
   
    // sdk
   
   	public static native void funcLoginResult(boolean isOK, String uid, String token, String type, String platform, String pltfmToken);
	public static native void funcPayResult(boolean isOK, String err);
	public static native void funcShareResult(boolean isOK, String err);
	public static native void funcInvitedFriendsResult(boolean isOK, HashMap<String, String>[] friends);
	public static native void funcFrindsResult(boolean isOK, HashMap<String, String>[] friends);
	public static native void funcSetAppRequestResult(int action, boolean isOK, String err);
    
    public static Object getInstance()
    {
        return sContext;
    }
    
    public void autoLogin(){
		// autoLogin
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.login(sContext, -1);
			}
		});
	}
    
	public void showMenu(final boolean showFlag, final int position){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.showMenu(sContext, position, showFlag);
			}
		});
	}
	
	public void toGoogleBillPayPage(final String productID, final double amountInCents, final String ext){
		// PAY
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.toGoogleBillPayPage(sContext, OASISPlatformConstant.REQUEST_CODE_GOOGLEPAY, productID, amountInCents, ext);
			}
		});
	}
	
	public void setUserInfo(String serverID, String serverName, String serverType, String username, String roleid){
		// setUserInfo
		OASISPlatform.setUserInfo(serverID,serverName,serverType, username, roleid);
	}
    
	public void switchUser(){
		// switchUser
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.switchUser(sContext);
			}
		});
	}
	
	public void cleanGameInfo(){
		// cleanGameInfo
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.cleanGameInfo(sContext);
			}
		});
	}
	
	public void trackEvent(final String eventToken, final HashMap<String,String> parameters){
		// trackEvent
		OASISPlatform.trackEvent(sContext, eventToken, parameters);
	}
	
	public void trackRevenue(final double amountInCents, final String transactionId, final String eventToken, final Map<String,String> parameters){
		// trackRevenue
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.trackRevenue(sContext, transactionId, amountInCents, eventToken, parameters);
			}
		});
	}
	
	public void share(HashMap<String, String> parameters){ //String link, String picture, String name, String caption, String description
		// share
		final String link = parameters.get("link");
		final String picture = parameters.get("picture");
		final String name = parameters.get("name");
		final String caption = parameters.get("caption");
		final String description = parameters.get("description");
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.shareByFacebook(sContext,link,picture,name,caption,description);
			}
		});
	}
	
	public void getFriends(final int limit, final boolean type){
		// getFriends
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.getFriends(sContext, limit, type);
			}
		});
	}
	
	public void getInvitableFriends(final int limit, final boolean type){
		// getInvitableFriends
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.getInvitableFriends(sContext, limit, type);
			}
		});
	}
	
	public void setAppRequest(final String actionType, final String uids,final  String message, final String objectid){
		// setAppRequest
		int t = 1;
		if (actionType == "send") {
			t = 2;
		} else if (actionType == "askfor") {
			t = 3;
		}
		final int type = t;
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OASISPlatform.setAppRequest(sContext, type, objectid, uids, message);
			}
		});
	}
	
	/**
	 * FB key hash
	 */
	private void getFBKeyHash(){

		try {
			PackageInfo info = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
			for (Signature signature : info.signatures) {
				MessageDigest md = MessageDigest.getInstance("SHA");
				md.update(signature.toByteArray());
				String sign = Base64.encodeToString(md.digest(), Base64.DEFAULT);
				Log.e("MY KEY HASH:", sign);
			}
		} catch (NameNotFoundException e) {
		} catch (NoSuchAlgorithmException e) {
		}
	}
	
	class OasisInterfaceImpl implements OASISPlatformInterface{

		@Override
		public void reloadGame(UserInfo userInfo) {
			if(userInfo == null){
//				funcLoginResult(false, "", "", "", "", "");
				return;
			}
			if (!("1".equals(userInfo.getOperation()))) {
				cleanGameInfo();
			}
			funcLoginResult(true, userInfo.uid, userInfo.token, userInfo.type+"", userInfo.platform, userInfo.platform_token);
			
		}

		@Override
		public void paymentCallback(String paymentWay, final int paymentCode, final String errorMessage) {
			Log.e("google pay", errorMessage);
			runOnGLThread(new Runnable() {
				@Override
				public void run() 
				{
					switch (paymentCode) {
					case OASISPlatformConstant.RESULT_SUCCESS:
						funcPayResult(true, "");
						break;
					case OASISPlatformConstant.RESULT_CANCLE:
					case OASISPlatformConstant.RESULT_FAIL:
					case OASISPlatformConstant.RESULT_EXCEPTION:
					case OASISPlatformConstant.RESULT_EXCEPTION_GOOGLEPAY_EXCEPTION:
					default:
						funcPayResult(false, errorMessage);
						break;
					}
				}
			});

		}
		
		@Override
		public void fbRequestCallback(int action, int resultCode, String id) {
			boolean isOk = true;
			if(resultCode != OASISPlatformConstant.RESULT_SUCCESS)
			{
				isOk = false;
			}
			switch (action) {
			case 0:
				// share
				funcShareResult(isOk, id);
				break;
			case 1:
				// invite
//				break;
			case 2:
				// send
				
//				break;
			case 3:
				// askfor
//				break;

			default:
				funcSetAppRequestResult(action, isOk, id);
				break;
			}
		}
		
		@SuppressWarnings("null")
		@Override
		public void fbFriendsListCallback(int type, int resultCode, FBPageInfo info) {
			
			HashMap<String, String>[] friends = null;
			boolean isOk = true;
			
			if(resultCode != OASISPlatformConstant.RESULT_SUCCESS)
			{
				isOk = false;
			} else
			{
				int i = 0;
				for (FriendInfo user : info.data) {
					HashMap<String, String> friend = new HashMap<String, String>();
					System.out.println("id="+user.getId()+"  name="+user.getName() +" \n "+user.picture);
					friend.put("id", user.getId());
					friend.put("name", user.getName());
					friend.put("picture", user.picture);
//					list.add(friend);
					friends[i] = friend;
					i++;
				}
			}

			if(type == OASISPlatformConstant.REQUEST_CODE_FACEBOOK_GETFRIENDS){
				// info 
				funcFrindsResult(isOk, friends);

			}
			else if(type == OASISPlatformConstant.REQUEST_CODE_FACEBOOK_GETINVITABLEFRIENDS){
				// info
				funcInvitedFriendsResult(isOk, friends);
			}

		}
		
		@Override
		public void getExtendValue(OasisCallback callback) {
			String extValue = null;
			extValue = "ext_value";
			callback.success(extValue);
//			callback.error("error");
		}
	}
   public void openUrl(String url)
   {
       Uri uri = Uri.parse(url);
       Intent it = new Intent(Intent.ACTION_VIEW, uri);
       startActivity(it);
   }
   
//   @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR1) 
   public double[] getScreenSizeOfDevice() {
//       Point point = new Point();
//       if(Build.VERSION.SDK_INT >= 17){
//       	sContext.getWindowManager().getDefaultDisplay().getRealSize(point);
//       }
//       else if(Build.VERSION.SDK_INT >= 13)
//       {
//       	sContext.getWindowManager().getDefaultDisplay().getSize(point);
//       } 
//       else
//       {
//       	point.x = sContext.getWindowManager().getDefaultDisplay().getWidth();
//       	point.y = sContext.getWindowManager().getDefaultDisplay().getHeight();
//       }
//       DisplayMetrics dm = getResources().getDisplayMetrics();
//       double x = point.x / dm.xdpi;
//       double y = point.y / dm.ydpi;
       // todo
	   DisplayMetrics metric = new DisplayMetrics();
	   getWindowManager().getDefaultDisplay().getMetrics(metric);
	   int width = metric.widthPixels;
	   int height = metric.heightPixels;
	   double x = width / metric.density;
	   double y = height / metric.density;
       double[] size = {x, y};
       return size;
   }
   
   public void setLockScreen(final boolean disabled)
   {
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
		       if (disabled) {
		          	getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		          } else {
		          	getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		          }
			}
		});

   }
   
   public String deviceIDFA() {
       
       final TelephonyManager tm = (TelephonyManager)getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
       
       final String tmDevice, tmSerial, androidId;
       tmDevice = "" + tm.getDeviceId();
       tmSerial = "" + tm.getSimSerialNumber();
       androidId = "" + android.provider.Settings.Secure.getString(getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
       
       UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
       String uniqueId = deviceUuid.toString();
       
       return uniqueId;
       
   }
   
   // notification
   public void pushMessage(final String content, final long delalt, final int repeats, final String key)
   {
       JSONObject j = new JSONObject();
       try {
           j.put("ticker", content);
//           j.put("title", title);
           j.put("text", content);
           j.put("repeats", repeats);
           j.put("delalt", delalt);
           j.put("key", key);
           j.put("packageName", "com.oasgames.android.mtsen");
           Cocos2dxAlarmManager.alarmNotify(sContext, j.toString());
       } catch (JSONException e) {
           // TODO Auto-generated catch block
           e.printStackTrace();
       }
   } 
   public void removeNotification(final String key)
   {
       int id = Integer.parseInt(key);
       Cocos2dxAlarmManager.cancelNotify(sContext, id);
   } 
   
   public void removeAllNotification()
   {
	   Cocos2dxAlarmManager.cancelAllNotify(sContext);
   } 
   
   public String getVersion() {
	   String versionName = "";
	   try {
		     PackageManager manager = this.getPackageManager();
		     PackageInfo info = manager.getPackageInfo(this.getPackageName(), 0);
		     versionName = info.versionName;  
//		     int versioncode = info.versionCode;
		     if (versionName == null || versionName.length() <= 0) {
		    	 return versionName;
		     } 
	   } catch (Exception e) {
		     e.printStackTrace();
		     return "";
	   }
	   return versionName;
   }
   
   public String getPkName() {
	   
	   String pkName = "";
	   try {
		   pkName = this.getPackageName();
		   if (pkName == null || pkName.length() <= 0) {
			   return pkName;
		   } 
	   } catch (Exception e) {
		     e.printStackTrace();
		     return "";
	   }
	   return pkName;
   }
   
   public String getLocalLanguage() {
	   String language = "";

       try {
    	   Locale locale = getResources().getConfiguration().locale;
           language = locale.getLanguage();
           if (language == null || language.length() <= 0) {
	    	 return language;
           } 
	   } catch (Exception e) {
		     e.printStackTrace();
		     return "";
	   }
       return language;
   }
   
   public String getContryCode() {
	   
	   String ret = "";
	   try {
		   final TelephonyManager tm = (TelephonyManager)getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
		   ret = tm.getSimCountryIso();
		   if (ret == null || ret.length() <= 0) {
		    	 return ret;
		   } 
	   } catch (Exception e) {
		     e.printStackTrace();
		     return "";
	   }
	   
	   return ret;			   
   }
   
   public boolean isEmulator() {
	   
       try{
           TelephonyManager tm = (TelephonyManager) getBaseContext().getSystemService(Context.TELEPHONY_SERVICE);
           String imei = tm.getDeviceId();
           if (imei != null && imei.equals("000000000000000")){
               return true;
           }
           return  (Build.MODEL.equals("sdk")) || (Build.MODEL.equals("google_sdk"));
       }catch (Exception ioe) { 

       }
       return false;
   }
}
